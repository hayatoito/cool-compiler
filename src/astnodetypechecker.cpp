#include "astnodetypechecker.hpp"
#include "constants.hpp"
#include "utility.hpp"

using namespace constants;

AstNodeTypeChecker::AstNodeTypeChecker(const std::map<ClassPtr, ClassPtr>& ig)
    : inherit_graph(ig)
{

}

bool AstNodeTypeChecker::is_subtype(const Symbol& child, const Symbol& parent)
{
    if (child == parent) return true;

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
            return *result;

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

        for (auto& feature : cs->features)
        {
            if (feature->get_type() == Feature::METHOD)
            {
                MethodPtr mptr = std::dynamic_pointer_cast<Method>(feature);
                
                for (auto& formal : mptr->params) 
                {
                    mtbl[cl][mptr->name].push_back(formal->type_decl);
                }

                std::cerr << "Adding [" << cl << "][" << mptr->name << "] -> " << mptr->return_type << "\n";
                mtbl[cl][mptr->name].push_back(mptr->return_type);
            }
        }
    }

    for (auto& cs : prog.classes)
        if (!utility::is_basic_class(cs->name))
            cs->accept(*this);
}

void AstNodeTypeChecker::visit(Class& cs)
{
    std::cerr << "entering class : " << cs.name << "\n";
    env.enter_scope();
    curr_class = cs.name;

    for (auto& f : cs.features)
        f->accept(*this);

    env.exit_scope();
    std::cerr << "leaving class : " << cs.name << "\n";
}

void AstNodeTypeChecker::visit(Feature& f)
{
    f.accept(*this);
}

void AstNodeTypeChecker::visit(Attribute& attr)
{
    env.add(attr.name, attr.type_decl);
    env.add(SELF, curr_class);
    attr.init->accept(*this);

    if (attr.init->type != NOTYPE)
        if (!is_subtype(attr.init->type, attr.type_decl))
            std::cerr << "Attribute initialization type mismatch.\n";
}

void AstNodeTypeChecker::visit(Method& method)
{
    env.enter_scope();

    for (auto& formals : method.params)
        env.add(formals->name, formals->type_decl);

    method.body->accept(*this);

    if (!is_subtype(method.body->type, method.return_type))
        std::cerr << "Method body type not a subtype of return type.\n";

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
    std::cerr << "**** in new node ****\n";
    nnode.type = nnode.type_decl;
}

void AstNodeTypeChecker::visit(IsVoid& isvoid)
{
    isvoid.expr->accept(*this);
    isvoid.type = BOOLEAN;
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
        std::cerr << "Variable " << assign.name << " not in scope.\n";

    assign.rhs->accept(*this);

    if (is_subtype(assign.rhs->type, *obj_type))
    {
        if (obj_type)
            assign.type = assign.rhs->type;
    }
    else
    {
        std::cerr << "Type of RHS not a subtype of variable being assigned to.\n";
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
        std::cerr << "Predicate of IF conditional must be of type Bool\n";

    ifstmt.iftrue->accept(*this);
    ifstmt.iffalse->accept(*this);
    ifstmt.type = lub(std::vector<Symbol> {ifstmt.iftrue->type, ifstmt.iffalse->type});
}

void AstNodeTypeChecker::visit(While& wstmt)
{
    wstmt.predicate->accept(*this);
    
    if (wstmt.predicate->type != BOOLEAN)
        std::cerr << "While statement predicate not boolean type.\n";

    wstmt.body->accept(*this);
    wstmt.type = OBJECT;
}

void AstNodeTypeChecker::visit(Complement& cmpl)
{
    cmpl.expr->accept(*this);
    cmpl.type = INTEGER;

    if (cmpl.expr->type != INTEGER)
        cmpl.type = OBJECT;
}

void AstNodeTypeChecker::visit(LessThan& lt)
{
    lt.lhs->accept(*this);
    lt.rhs->accept(*this);
    lt.type = BOOLEAN;

    if (lt.lhs->type != INTEGER || lt.rhs->type != INTEGER)
    {
        std::cerr << "LHS or RHS of comparison operator not of type INT\n";
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
            std::cerr << "Comparison of primitives INT, BOOL, and STRING must be of same type\n";
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
        std::cerr << "LHS or RHS of comparison operator not of type INT\n";
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
        std::cerr << "Operands of arithmetic expression not of type INT\n";
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
        std::cerr << "Operands of arithmetic expression not of type INT\n";
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
        std::cerr << "Operands of arithmetic expression not of type INT\n";
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
        std::cerr << "Operands of arithmetic expression not of type INT\n";
        div.type = OBJECT;
    }
}

void AstNodeTypeChecker::visit(Not& nt)
{
    nt.expr->accept(*this);
    nt.type = BOOLEAN;

    if (nt.expr->type != BOOLEAN)
    {
        std::cerr << "Expression in not node not of boolean type\n";
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
        std::cerr << obj_type << " is not a subtype of static dispatch type " << stat.type_decl << "\n";
        stat.type = OBJECT;
    }
    
    // check if each dispatch argument's type is a subtype of declared type for method
    bool result = std::equal(begin(disptypes), end(disptypes), begin(mtbl[curr_class][stat.method]),
            [&](const Symbol& t1, const Symbol& t2) {
                return is_subtype(t1, t2);
            });

    if (result)
    {
        if (statsub)
            stat.type = mtbl[stat.type][stat.method].back();
    }
    else
    {
        std::cerr << "Type mismatch for dispatch to method " << stat.method << "\n";
        stat.type = OBJECT;
    }

}

void AstNodeTypeChecker::visit(DynamicDispatch& dyn)
{
    std::cerr << "in dynamic dispatch\n";
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

    std::cerr << "before equal\n";
    // check if each dispatch argument's type is a subtype of declared type for method
    bool result = std::equal(begin(disptypes), end(disptypes), begin(mtbl[curr_class][dyn.method]),
            [&](const Symbol& t1, const Symbol& t2) {
                return is_subtype(t1, t2);
            });

    std::cerr << "after equal\n";

    if (result)
    {
        std::cerr << "Trying to access back() of [" << curr_class << "][" << dyn.method << "]\n";
        dyn.type = mtbl[obj_type][dyn.method].back();
    }
    else
    {
        std::cerr << "cp2\n";
        std::cerr << "Type mismatch for dispatch to method " << dyn.method << "\n";
        dyn.type = OBJECT;
    }
}

void AstNodeTypeChecker::visit(Let& let)
{
    std::cerr << "**** in let ****\n";
    let.type = OBJECT;
    let.init->accept(*this);

    bool type_status = true;

    if (let.init->type != NOTYPE)
    {
        std::cerr << "**** before subtype ****\n";
        type_status = is_subtype(let.init->type, let.type_decl);
        std::cerr << "**** after subtype ****\n";
        if (!type_status)
            std::cerr << "Let init type mismatch\n";
    }
    std::cerr << "**** after if ****\n";

    env.enter_scope();
    env.add(let.name, let.type_decl);
    std::cerr << "**** before body ****\n";
    let.body->accept(*this);
    std::cerr << "**** after body accept ****\n";

    if (type_status)
        let.type = let.body->type;

    env.exit_scope();
    std::cerr << "**** out let ****\n";
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
    var.type = obj_type ? *obj_type : OBJECT;
}

void AstNodeTypeChecker::visit(NoExpr& ne)
{
    ne.type = NOTYPE;
}

