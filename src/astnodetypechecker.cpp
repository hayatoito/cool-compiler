#include "astnodetypechecker.hpp"
#include "constants.hpp"

using namespace constants;

void AstNodeTypeChecker::visit(Program& prog)
{
    for (auto& cs : prog.classes)
        cs->accept(*this);
}

void AstNodeTypeChecker::visit(Class& cs)
{
    env.enter_scope();

    for (auto& f : cs.features)
        f->accept(*this);

    env.exit_scope();
}

void AstNodeTypeChecker::visit(Feature&)
{

}

void AstNodeTypeChecker::visit(Attribute&)
{

}

void AstNodeTypeChecker::visit(Method&)
{

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
}

void AstNodeTypeChecker::visit(CaseBranch&)
{

}

void AstNodeTypeChecker::visit(Assign&)
{

}

void AstNodeTypeChecker::visit(Block& block)
{
    for (auto& expr : block.body)
        expr->accept(*this);

    block.type = block.body.back()->type;
}

void AstNodeTypeChecker::visit(If& ifstmt)
{

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

void AstNodeTypeChecker::visit(StaticDispatch&)
{

}

void AstNodeTypeChecker::visit(DynamicDispatch&)
{

}

void AstNodeTypeChecker::visit(Let&)
{

}

void AstNodeTypeChecker::visit(Case&)
{

}

void AstNodeTypeChecker::visit(Object& var)
{
    boost::optional<Symbol> obj_type = env.lookup(var.name);
    var.type = obj_type ? *obj_type : OBJECT;
}

void AstNodeTypeChecker::visit(NoExpr&)
{

}

