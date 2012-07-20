#include "astnodevisitor.hpp"

AstNodePrintVisitor::AstNodePrintVisitor(std::ostream& stream)
    : os(stream), depth(0)
{

}

void AstNodePrintVisitor::visit(const Program& prog)
{
    for (auto& cs : prog.classes)
        cs->accept(*this);
}

void AstNodePrintVisitor::visit(const Class& cs)
{
    os << std::setw(depth++) << "";
    os << "-class (" << cs.name << ")\n";  
    
    for (auto& feature : cs.features)
        feature->accept(*this);

    --depth;
}

void AstNodePrintVisitor::visit(const Attribute& attr)
{
    os << std::setw(depth++) << "";
    os << "-attribute (" << attr.name << ")\n";
    attr.init->accept(*this);
    --depth;
}

void AstNodePrintVisitor::visit(const Feature& feature)
{
    feature.accept(*this);
}

void AstNodePrintVisitor::visit(const Formal& formal) 
{ 
    os << std::setw(depth) << "";
    os << "-formal (" << formal.name << ")\n";
}

void AstNodePrintVisitor::visit(const Method& method) 
{ 
    os << std::setw(depth++) << "";
    os << "-method (" << method.name << ")\n";

    for (auto& formal : method.params)
        formal->accept(*this); 

    method.body->accept(*this);
    --depth;
}

void AstNodePrintVisitor::visit(const StringConst& str) 
{ 
    os << std::setw(depth) << "";
    os << "-str_const (" << str.token << ")\n";
}

void AstNodePrintVisitor::visit(const IntConst& int_const) 
{
    os << std::setw(depth) << "";
    os << "-int_const (" << int_const.token << ")\n";
}

void AstNodePrintVisitor::visit(const BoolConst& bool_const) 
{ 
    os << std::setw(depth) << "";
    os << "-bool_const (" << bool_const.value << ")\n";
}

void AstNodePrintVisitor::visit(const New& new_node) 
{
    os << std::setw(depth) << "";
    os << "-new\n";
}

void AstNodePrintVisitor::visit(const IsVoid& isvoid) 
{ 
    os << std::setw(depth++) << "";
    os << "-isvoid\n";
    isvoid.expr->accept(*this); 
    --depth;
}

void AstNodePrintVisitor::visit(const CaseBranch& branch) 
{ 
    os << std::setw(depth++) << "";
    os << "-casebranch\n";
    branch.expr->accept(*this);
    --depth;
}

void AstNodePrintVisitor::visit(const Assign& assign) 
{ 
    os << std::setw(depth++) << "";
    os << "-assign\n";
    assign.rhs->accept(*this);
    --depth;
}

void AstNodePrintVisitor::visit(const Block& block) 
{ 
    os << std::setw(depth++) << "";
    os << "-block\n";

    for (auto& expr : block.body)
        expr->accept(*this);
    --depth;
}

void AstNodePrintVisitor::visit(const If& ifstmt) 
{ 
    os << std::setw(depth++) << "";
    os << "-if\n";

    ifstmt.predicate->accept(*this);
    ifstmt.iftrue->accept(*this);
    ifstmt.iffalse->accept(*this);
    --depth;
}

void AstNodePrintVisitor::visit(const While& whilestmt) 
{ 
    os << std::setw(depth++) << "";
    os << "-while\n";

    whilestmt.predicate->accept(*this);
    whilestmt.body->accept(*this);
    --depth;
}

void AstNodePrintVisitor::visit(const Complement& comp) 
{ 
    os << std::setw(depth++) << "";
    os << "-comp\n";
    comp.expr->accept(*this);
    --depth;
}

void AstNodePrintVisitor::visit(const LessThan& lt) 
{ 
    os << std::setw(depth++) << "";
    os << "-lt\n";
    lt.lhs->accept(*this);
    lt.rhs->accept(*this);
    --depth;
}

void AstNodePrintVisitor::visit(const EqualTo& eq) 
{ 
    os << std::setw(depth++) << "";
    os << "-eq\n";
    eq.lhs->accept(*this);
    eq.rhs->accept(*this);
    --depth;
}

void AstNodePrintVisitor::visit(const LessThanEqualTo& lteq) 
{ 
    os << std::setw(depth++) << "";
    os << "-lteq\n";
    lteq.lhs->accept(*this);
    lteq.rhs->accept(*this);
    --depth;
}

void AstNodePrintVisitor::visit(const Plus& plus) 
{
    os << std::setw(depth++) << "";
    os << "-plus\n";
    plus.lhs->accept(*this);
    plus.rhs->accept(*this);
    --depth;
}

void AstNodePrintVisitor::visit(const Sub& sub) 
{ 
    os << std::setw(depth++) << "";
    os << "-sub\n";
    sub.lhs->accept(*this);
    sub.rhs->accept(*this);
    --depth;
}

void AstNodePrintVisitor::visit(const Mul& mul) 
{ 
    os << std::setw(depth++) << "";
    os << "-mul\n";
    mul.lhs->accept(*this);
    mul.rhs->accept(*this);
    --depth;
}

void AstNodePrintVisitor::visit(const Div& div) 
{ 
    os << std::setw(depth++) << "";
    os << "-div\n";
    div.lhs->accept(*this);
    div.rhs->accept(*this);
    --depth;
}

void AstNodePrintVisitor::visit(const Not& nt) 
{ 
    os << std::setw(depth++) << "";
    os << "-not\n";
    nt.expr->accept(*this);
    --depth;
}

void AstNodePrintVisitor::visit(const StaticDispatch& sdisp) 
{ 
    os << std::setw(depth++) << "";
    os << "-static_dispatch\n";

    sdisp.obj->accept(*this);
    for (auto& e : sdisp.actual)
       e->accept(*this); 
    --depth;
}

void AstNodePrintVisitor::visit(const DynamicDispatch& ddisp) 
{ 
    os << std::setw(depth++) << "";
    os << "-dynamic_dispatch\n";

    ddisp.obj->accept(*this);
    for (auto& e : ddisp.actual)
       e->accept(*this); 
    --depth;
}

void AstNodePrintVisitor::visit(const Let& let) 
{ 
    os << std::setw(depth++) << "";
    os << "-let\n";
    
    let.init->accept(*this);
    let.body->accept(*this);
    --depth;
}

void AstNodePrintVisitor::visit(const Case& caze) 
{ 
    os << std::setw(depth++) << "";
    os << "-case\n";

    caze.expr->accept(*this);
    for (auto& br : caze.branches)
        br->accept(*this);
    --depth;
}

void AstNodePrintVisitor::visit(const Object& obj) 
{ 
    os << std::setw(depth) << "";
    os << "-object\n";
}

void AstNodePrintVisitor::visit(const NoExpr&) 
{ 
    os << std::setw(depth) << "";
    os << "-noexpr\n";
}
