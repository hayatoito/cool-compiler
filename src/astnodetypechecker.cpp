#include "astnodetypechecker.hpp"
#include "constants.hpp"
#include "utility.hpp"

#include <functional>
#include <sstream>

using namespace constants;

AstNodeTypeChecker::AstNodeTypeChecker(const std::map<ClassPtr, ClassPtr>& ig)
    : inherit_graph(ig), err_count(0)
{

}

std::size_t AstNodeTypeChecker::get_err_count() const
{
    return err_count;
}

void AstNodeTypeChecker::error(const AstNode& node, const std::string& msg)
{
    utility::print_error(node, msg);
    ++err_count;
}

bool AstNodeTypeChecker::is_subtype(const Symbol& child, const Symbol& parent)
{
    if (child == NOTYPE || child == parent) return true;
    if (child == OBJECT) return false;
    if (child == SELF_TYPE || parent == SELF_TYPE) return true;

    auto child_cptr = std::find_if(begin(inherit_graph), end(inherit_graph),
            [&](const std::pair<ClassPtr, ClassPtr>& p) {
                return p.first->name == child;
            }); 

    ClassPtr curr = child_cptr->first;
    while(curr->name != OBJECT) 
    {
        if (curr->name == parent)
            return true;

        curr = inherit_graph[curr];
    }

    return false;
}

Symbol AstNodeTypeChecker::lub(const std::vector<Symbol>& types)
{
    Symbol base = types.front(); 

    if (std::all_of(begin(types) + 1, end(types), std::bind2nd(std::equal_to<Symbol>(), base)))
        return base;

    auto base_ptr = std::find_if(begin(inherit_graph), end(inherit_graph),
            [&](const std::pair<ClassPtr, ClassPtr>& p) {
                return p.first->name == base;
            }); 

    ClassPtr curr = base_ptr->first;

    while (curr->parent != OBJECT)
    {
        auto result = std::find_if(begin(types), end(types), 
                [&](const Symbol& s) {
                    return !is_subtype(s, curr->parent);
                });

        if (result == end(types))
            return curr->parent;

        curr = inherit_graph[curr];
    }

    return OBJECT;
}

void AstNodeTypeChecker::visit(Program& prog)
{
    // populate method table with [class][method] -> argument types, return type
    for (auto& cs : prog.classes)
    {
        Symbol cl = cs->name;
        ClassPtr curr = cs;

        while (curr->parent != NOCLASS)
        {
            for (auto& method : curr->methods)
            {
                // if an overriden method already exists, don't add anymore
                if (mtbl[cl].find(method->name) == end(mtbl[cl]))
                {
                    for (auto& formal : method->params) 
                        mtbl[cl][method->name].push_back(formal->type_decl);

                    mtbl[cl][method->name].push_back(method->return_type);
                }
                else
                {
                    // if there is an overriden version of this method, type check
                    // to ensure that the signature including return type is exactly same 
                    
                    // have to subtract 1 from mtbl because the last element is the return type (ie. it has one extra element)
                    if (method->params.size() != mtbl[cl][method->name].size() - 1 || 
                            !std::equal(begin(method->params), end(method->params), begin(mtbl[cl][method->name]), 
                                    [](const FormalPtr& f, const Symbol& s) {
                                        return f->type_decl == s; 
                                    }))
                    {
                        std::ostringstream oss;
                        oss << "overriden method " << curr->name << "." << method->name 
                            << " has different parameters from " << cl << "." << method->name;   
                        error(*method, oss.str());
                    }

                    if (method->return_type != mtbl[cl][method->name].back())
                    {
                        std::ostringstream oss;
                        oss << "overriden method " << curr->name << "." << method->name
                            << " has different return type from " << cl << "." << method->name << "\n";
                        error(*method, oss.str());
                    }
                }
            }

            curr = inherit_graph[curr];
        }
    }

    for (auto& cs : prog.classes)
        cs->accept(*this);
}

void AstNodeTypeChecker::visit(Class& cs)
{
    env.enter_scope();
    curr_class = cs.name;
    env.add(SELF, curr_class);

    auto cs_ptr = std::find_if(begin(inherit_graph), end(inherit_graph),
            [&](const std::pair<ClassPtr, ClassPtr>& p) {
                return cs.parent == p.first->name;
            });

    if (cs_ptr != end(inherit_graph))
    {
        ClassPtr cptr = cs_ptr->first;

        while (cptr->name != OBJECT)
        {
            for (auto& attrib : cptr->attributes)
            {
                if (env.probe(attrib->name))
                    error(*attrib, "attribute " + attrib->name.get_val() + " redefined in one of its subclasses");
                else
                    env.add(attrib->name, attrib->type_decl);
            }

            cptr = inherit_graph[cptr];
        }
    }

    for (auto& attrib : cs.attributes)
        attrib->accept(*this);

    for (auto& method : cs.methods)
        method->accept(*this);

    env.exit_scope();
}

void AstNodeTypeChecker::visit(Attribute& attr)
{
    env.add(attr.name, attr.type_decl);
    attr.init->accept(*this);

    if (attr.init->type != NOTYPE)
        if (!is_subtype(attr.init->type, attr.type_decl))
            error(attr, "type of attribute initializer not a subtype of declared type");
}

void AstNodeTypeChecker::visit(Method& method)
{
    env.enter_scope();

    for (auto& formals : method.params)
        env.add(formals->name, formals->type_decl);

    method.body->accept(*this);

    if (!is_subtype(method.body->type, method.return_type))
        error(method, "method body type not a subtype of return type");

    env.exit_scope();
}

void AstNodeTypeChecker::visit(Formal&)
{

}

void AstNodeTypeChecker::visit(StringConst& snode)
{
    snode.type = STRING;
}

void AstNodeTypeChecker::visit(IntConst& inode)
{
    inode.type = INTEGER;
}

void AstNodeTypeChecker::visit(BoolConst& boolean)
{
    boolean.type = BOOLEAN;
}

void AstNodeTypeChecker::visit(New& nnode)
{
    nnode.type = nnode.type_decl;
}

void AstNodeTypeChecker::visit(IsVoid& isvoid)
{
    isvoid.expr->accept(*this);
    isvoid.type = BOOLEAN;

    if (isvoid.expr->type == OBJECT)
    {
        error(isvoid, "isvoid expression doesn't evaluate to type Bool");
        isvoid.type = OBJECT;
    }
}

void AstNodeTypeChecker::visit(CaseBranch& br)
{
    br.expr->accept(*this);
    br.type = br.expr->type;
}

void AstNodeTypeChecker::visit(Assign& assign)
{
    boost::optional<Symbol> obj_type = env.lookup(assign.name);
    assign.type = OBJECT;

    if (!obj_type)
        error(assign, "variable " + assign.name.get_val() + " not in scope");

    assign.rhs->accept(*this);

    if (is_subtype(assign.rhs->type, *obj_type))
    {
        if (obj_type)
            assign.type = assign.rhs->type;
    }
    else
    {
        error(assign, "type of RHS not a subtype of variable type");
    }
}

void AstNodeTypeChecker::visit(Block& block)
{
    for (auto& expr : block.body)
        expr->accept(*this);

    block.type = block.body.back()->type;
}

void AstNodeTypeChecker::visit(If& ifstmt)
{
    ifstmt.predicate->accept(*this);

    if (ifstmt.predicate->type != BOOLEAN)
        error(ifstmt, "predicate doesn't evaluate to type Bool");

    ifstmt.iftrue->accept(*this);
    ifstmt.iffalse->accept(*this);

    ifstmt.type = lub(std::vector<Symbol> {ifstmt.iftrue->type, ifstmt.iffalse->type});
}

void AstNodeTypeChecker::visit(While& wstmt)
{
    wstmt.predicate->accept(*this);
    
    if (wstmt.predicate->type != BOOLEAN)
        error(wstmt, "predicate doesn't evaluate to type Bool");

    wstmt.body->accept(*this);
    wstmt.type = OBJECT;
}

void AstNodeTypeChecker::visit(Complement& cmpl)
{
    cmpl.expr->accept(*this);
    cmpl.type = INTEGER;

    if (cmpl.expr->type != INTEGER)
    {
        error(cmpl, "RHS of expression must evaluate to type Int");
        cmpl.type = OBJECT;
    }
}

void AstNodeTypeChecker::visit(LessThan& lt)
{
    lt.lhs->accept(*this);
    lt.rhs->accept(*this);
    lt.type = BOOLEAN;

    if (lt.lhs->type != INTEGER || lt.rhs->type != INTEGER)
    {
        error(lt, "LHS or RHS of comparison operator not of type Int");
        lt.type = OBJECT;
    }
}

void AstNodeTypeChecker::visit(EqualTo& eq)
{
    eq.lhs->accept(*this);
    eq.rhs->accept(*this);
    eq.type = BOOLEAN;

    Symbol lhs_type(eq.lhs->type);
    Symbol rhs_type(eq.rhs->type);

    if (lhs_type == INTEGER || lhs_type == BOOLEAN || lhs_type == STRING ||
        rhs_type == INTEGER || rhs_type == BOOLEAN || rhs_type == STRING)
    {
        if (lhs_type != rhs_type)
        {
            error(eq, "comparison of primitives Int, Bool, and String must be of same type");
            eq.type = OBJECT;
        }
    }
}

void AstNodeTypeChecker::visit(LessThanEqualTo& lte)
{
    lte.lhs->accept(*this);
    lte.rhs->accept(*this);
    lte.type = BOOLEAN;

    if (lte.lhs->type != INTEGER || lte.rhs->type != INTEGER)
    {
        error(lte, "LHS or RHS of comparison operator not of type Int");
        lte.type = OBJECT;
    }
}

void AstNodeTypeChecker::visit(Plus& plus)
{
    plus.lhs->accept(*this);
    plus.rhs->accept(*this);
    plus.type = INTEGER;

    if (plus.lhs->type != INTEGER || plus.rhs->type != INTEGER)
    {
        error(plus, "operands of arithmetic expression not of type Int");
        plus.type = OBJECT;
    }
}

void AstNodeTypeChecker::visit(Sub& sub)
{
    sub.lhs->accept(*this);
    sub.rhs->accept(*this);
    sub.type = INTEGER;

    if (sub.lhs->type != INTEGER || sub.rhs->type != INTEGER)
    {
        error(sub, "operands of arithmetic expression not of type Int");
        sub.type = OBJECT;
    }
}

void AstNodeTypeChecker::visit(Mul& mul)
{
    mul.lhs->accept(*this);
    mul.rhs->accept(*this);
    mul.type = INTEGER;

    if (mul.lhs->type != INTEGER || mul.rhs->type != INTEGER)
    {
        error(mul, "operands of arithmetic expression not of type Int");
        mul.type = OBJECT;
    }
}

void AstNodeTypeChecker::visit(Div& div)
{
    div.lhs->accept(*this);
    div.rhs->accept(*this);
    div.type = INTEGER;

    if (div.lhs->type != INTEGER || div.rhs->type != INTEGER)
    {
        error(div, "operands of arithmetic expression not of type Int");
        div.type = OBJECT;
    }
}

void AstNodeTypeChecker::visit(Not& nt)
{
    nt.expr->accept(*this);
    nt.type = BOOLEAN;

    if (nt.expr->type != BOOLEAN)
    {
        error(nt, "not expression does not evaluate to Bool");
        nt.type = OBJECT;
    }
}

void AstNodeTypeChecker::visit(StaticDispatch& stat)
{
    std::vector<Symbol> disptypes;

    // get type of object that dispatches
    stat.obj->accept(*this);
    Symbol obj_type = stat.obj->type;

    // get types of all arguments to dispatch
    for (auto& expr : stat.actual)
    { 
        expr->accept(*this);
        disptypes.push_back(expr->type);
    }

    bool statsub = is_subtype(obj_type, stat.type_decl);
    
    if (!statsub)
    {
        error(stat, "dispatch object type not a subtype of static dispatch type");
        stat.type = OBJECT;
    }

    // check if each dispatch argument's type is a subtype of declared type for method
    bool result = std::equal(begin(disptypes), end(disptypes), begin(mtbl[stat.type_decl][stat.method]),
            [&](const Symbol& t1, const Symbol& t2) {
                return is_subtype(t1, t2);
            });

    if (result)
    {
        if (statsub)
            stat.type = mtbl[stat.type_decl][stat.method].back();
    }
    else
    {
        error(stat, "type mismatch in one of the arguments of the dispatch");
        stat.type = OBJECT;
    }
}

void AstNodeTypeChecker::visit(DynamicDispatch& dyn)
{
    std::vector<Symbol> disptypes;

    // get type of object that dispatches
    dyn.obj->accept(*this);
    Symbol obj_type = dyn.obj->type;

    // get types of all arguments to dispatch
    for (auto& expr : dyn.actual)
    { 
        expr->accept(*this);
        disptypes.push_back(expr->type);
    }

    if (obj_type == curr_class)
        obj_type = curr_class;

    // check if each dispatch argument's type is a subtype of declared type for method
    bool result = std::equal(begin(disptypes), end(disptypes), begin(mtbl[obj_type][dyn.method]),
            [&](const Symbol& t1, const Symbol& t2) {
                return is_subtype(t1, t2);
            });

    if (result)
    {
        dyn.type = mtbl[obj_type][dyn.method].back();
    }
    else
    {
        error(dyn, "type mismatch in one of the arguments of the dispatch");
        dyn.type = OBJECT;
    }
}

void AstNodeTypeChecker::visit(Let& let)
{
    let.type = OBJECT;
    let.init->accept(*this);

    bool type_status = true;

    if (let.init->type != NOTYPE)
    {
        type_status = is_subtype(let.init->type, let.type_decl);
        if (!type_status)
            error(let, "initialization of " + let.name.get_val() + " not a subtype of declared type");
    }

    env.enter_scope();
    env.add(let.name, let.type_decl);
    let.body->accept(*this);

    if (type_status)
        let.type = let.body->type;

    env.exit_scope();
}

void AstNodeTypeChecker::visit(Case& cs)
{
    cs.expr->accept(*this);

    std::vector<Symbol> types;

    for (auto& br : cs.branches)
    {
        env.enter_scope();
        env.add(br->name, br->type_decl);
        br->accept(*this);
        types.push_back(br->type);
        env.exit_scope();
    }

    cs.type = lub(types);
}

void AstNodeTypeChecker::visit(Object& var)
{
    boost::optional<Symbol> obj_type = env.lookup(var.name);
    var.type = OBJECT;

    if (obj_type)
        var.type = *obj_type;
    else
        error(var, "variable " + var.name.get_val() + " not in scope");
}

void AstNodeTypeChecker::visit(NoExpr& ne)
{
    ne.type = NOTYPE;
}

