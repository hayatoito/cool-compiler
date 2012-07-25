#include "astnodevisitor.hpp"
#include <iomanip>

AstNodeDisplayer::AstNodeDisplayer(std::ostream& stream)
    : os(stream), depth(0)
{

}

void AstNodeDisplayer::visit(const Program& prog)
{
    for (auto& cs : prog.classes)
        cs->accept(*this);
}

void AstNodeDisplayer::visit(const Class& cs)
{
    os << std::setw(depth++) << "";
    os << "-class (" << cs.name << ")\n";  
    
    for (auto& feature : cs.features)
        feature->accept(*this);

    --depth;
}

void AstNodeDisplayer::visit(const Attribute& attr)
{
    os << std::setw(depth++) << "";
    os << "-attribute (" << attr.name << ")\n";
    attr.init->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(const Feature& feature)
{
    feature.accept(*this);
}

void AstNodeDisplayer::visit(const Formal& formal) 
{ 
    os << std::setw(depth) << "";
    os << "-formal (" << formal.name << ")\n";
}

void AstNodeDisplayer::visit(const Method& method) 
{ 
    os << std::setw(depth++) << "";
    os << "-method (" << method.name << ")\n";

    for (auto& formal : method.params)
        formal->accept(*this); 

    method.body->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(const StringConst& str) 
{ 
    os << std::setw(depth) << "";
    os << "-str_const (" << str.token << ")\n";
}

void AstNodeDisplayer::visit(const IntConst& int_const) 
{
    os << std::setw(depth) << "";
    os << "-int_const (" << int_const.token << ")\n";
}

void AstNodeDisplayer::visit(const BoolConst& bool_const) 
{ 
    os << std::setw(depth) << "";
    os << "-bool_const (" << bool_const.value << ")\n";
}

void AstNodeDisplayer::visit(const New& new_node) 
{
    os << std::setw(depth) << "";
    os << "-new\n";
}

void AstNodeDisplayer::visit(const IsVoid& isvoid) 
{ 
    os << std::setw(depth++) << "";
    os << "-isvoid\n";
    isvoid.expr->accept(*this); 
    --depth;
}

void AstNodeDisplayer::visit(const CaseBranch& branch) 
{ 
    os << std::setw(depth++) << "";
    os << "-casebranch\n";
    branch.expr->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(const Assign& assign) 
{ 
    os << std::setw(depth++) << "";
    os << "-assign\n";
    assign.rhs->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(const Block& block) 
{ 
    os << std::setw(depth++) << "";
    os << "-block\n";

    for (auto& expr : block.body)
        expr->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(const If& ifstmt) 
{ 
    os << std::setw(depth++) << "";
    os << "-if\n";

    ifstmt.predicate->accept(*this);
    ifstmt.iftrue->accept(*this);
    ifstmt.iffalse->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(const While& whilestmt) 
{ 
    os << std::setw(depth++) << "";
    os << "-while\n";

    whilestmt.predicate->accept(*this);
    whilestmt.body->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(const Complement& comp) 
{ 
    os << std::setw(depth++) << "";
    os << "-comp\n";
    comp.expr->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(const LessThan& lt) 
{ 
    os << std::setw(depth++) << "";
    os << "-lt\n";
    lt.lhs->accept(*this);
    lt.rhs->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(const EqualTo& eq) 
{ 
    os << std::setw(depth++) << "";
    os << "-eq\n";
    eq.lhs->accept(*this);
    eq.rhs->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(const LessThanEqualTo& lteq) 
{ 
    os << std::setw(depth++) << "";
    os << "-lteq\n";
    lteq.lhs->accept(*this);
    lteq.rhs->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(const Plus& plus) 
{
    os << std::setw(depth++) << "";
    os << "-plus\n";
    plus.lhs->accept(*this);
    plus.rhs->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(const Sub& sub) 
{ 
    os << std::setw(depth++) << "";
    os << "-sub\n";
    sub.lhs->accept(*this);
    sub.rhs->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(const Mul& mul) 
{ 
    os << std::setw(depth++) << "";
    os << "-mul\n";
    mul.lhs->accept(*this);
    mul.rhs->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(const Div& div) 
{ 
    os << std::setw(depth++) << "";
    os << "-div\n";
    div.lhs->accept(*this);
    div.rhs->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(const Not& nt) 
{ 
    os << std::setw(depth++) << "";
    os << "-not\n";
    nt.expr->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(const StaticDispatch& sdisp) 
{ 
    os << std::setw(depth++) << "";
    os << "-static_dispatch\n";

    sdisp.obj->accept(*this);
    for (auto& e : sdisp.actual)
       e->accept(*this); 
    --depth;
}

void AstNodeDisplayer::visit(const DynamicDispatch& ddisp) 
{ 
    os << std::setw(depth++) << "";
    os << "-dynamic_dispatch\n";

    ddisp.obj->accept(*this);
    for (auto& e : ddisp.actual)
       e->accept(*this); 
    --depth;
}

void AstNodeDisplayer::visit(const Let& let) 
{ 
    os << std::setw(depth++) << "";
    os << "-let\n";
    
    let.init->accept(*this);
    let.body->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(const Case& caze) 
{ 
    os << std::setw(depth++) << "";
    os << "-case\n";

    caze.expr->accept(*this);
    for (auto& br : caze.branches)
        br->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(const Object& obj) 
{ 
    os << std::setw(depth) << "";
    os << "-object\n";
}

void AstNodeDisplayer::visit(const NoExpr&) 
{ 
    os << std::setw(depth) << "";
    os << "-noexpr\n";
}


//Code generation implementation
AstNodeCodeGenerator::AstNodeCodeGenerator(const std::map<std::string, std::string>& ig, 
        std::ostream& stream)
    : inherit_graph(ig), os(stream) 
{

}

void AstNodeCodeGenerator::emit_addiu(const char* dst, const char* src1, int imm)
{
    os << "addiu\t$" << dst << ", $" << src1 << ", " << imm << "\n";
}

void AstNodeCodeGenerator::visit(const Program& prog)
{
    for (auto& cs : prog.classes)
        cs->accept(*this);
}

void AstNodeCodeGenerator::visit(const Class& cs)
{
    for (auto& feature : cs.features)
        feature->accept(*this);
}

void AstNodeCodeGenerator::visit(const Attribute& attr)
{
    attr.init->accept(*this);
}

void AstNodeCodeGenerator::visit(const Feature& feature)
{
    feature.accept(*this);
}

void AstNodeCodeGenerator::visit(const Formal& formal) 
{ 

}

void AstNodeCodeGenerator::visit(const Method& method) 
{ 
    for (auto& formal : method.params)
        formal->accept(*this); 

    method.body->accept(*this);
}

void AstNodeCodeGenerator::visit(const StringConst& str) 
{ 

}

void AstNodeCodeGenerator::visit(const IntConst& int_const) 
{

}

void AstNodeCodeGenerator::visit(const BoolConst& bool_const) 
{ 
    
}

void AstNodeCodeGenerator::visit(const New& new_node) 
{

}

void AstNodeCodeGenerator::visit(const IsVoid& isvoid) 
{ 
    isvoid.expr->accept(*this); 
}

void AstNodeCodeGenerator::visit(const CaseBranch& branch) 
{ 
    branch.expr->accept(*this);
}

void AstNodeCodeGenerator::visit(const Assign& assign) 
{ 
    assign.rhs->accept(*this);
}

void AstNodeCodeGenerator::visit(const Block& block) 
{ 
    for (auto& expr : block.body)
        expr->accept(*this);
}

void AstNodeCodeGenerator::visit(const If& ifstmt) 
{ 
    ifstmt.predicate->accept(*this);
    ifstmt.iftrue->accept(*this);
    ifstmt.iffalse->accept(*this);
}

void AstNodeCodeGenerator::visit(const While& whilestmt) 
{ 
    whilestmt.predicate->accept(*this);
    whilestmt.body->accept(*this);
}

void AstNodeCodeGenerator::visit(const Complement& comp) 
{ 
    comp.expr->accept(*this);
}

void AstNodeCodeGenerator::visit(const LessThan& lt) 
{ 
    lt.lhs->accept(*this);
    lt.rhs->accept(*this);
}

void AstNodeCodeGenerator::visit(const EqualTo& eq) 
{ 
    eq.lhs->accept(*this);
    eq.rhs->accept(*this);
}

void AstNodeCodeGenerator::visit(const LessThanEqualTo& lteq) 
{ 
    lteq.lhs->accept(*this);
    lteq.rhs->accept(*this);
}

void AstNodeCodeGenerator::visit(const Plus& plus) 
{
    plus.lhs->accept(*this);
    plus.rhs->accept(*this);
}

void AstNodeCodeGenerator::visit(const Sub& sub) 
{ 
    sub.lhs->accept(*this);
    sub.rhs->accept(*this);
}

void AstNodeCodeGenerator::visit(const Mul& mul) 
{ 
    mul.lhs->accept(*this);
    mul.rhs->accept(*this);
}

void AstNodeCodeGenerator::visit(const Div& div) 
{ 
    div.lhs->accept(*this);
    div.rhs->accept(*this);
}

void AstNodeCodeGenerator::visit(const Not& nt) 
{ 
    nt.expr->accept(*this);
}

void AstNodeCodeGenerator::visit(const StaticDispatch& sdisp) 
{ 
    sdisp.obj->accept(*this);
    for (auto& e : sdisp.actual)
       e->accept(*this); 
}

void AstNodeCodeGenerator::visit(const DynamicDispatch& ddisp) 
{ 
    ddisp.obj->accept(*this);
    for (auto& e : ddisp.actual)
       e->accept(*this); 
}

void AstNodeCodeGenerator::visit(const Let& let) 
{ 
    let.init->accept(*this);
    let.body->accept(*this);
}

void AstNodeCodeGenerator::visit(const Case& caze) 
{ 
    caze.expr->accept(*this);
    for (auto& br : caze.branches)
        br->accept(*this);
}

void AstNodeCodeGenerator::visit(const Object& obj) 
{ 
    
}

void AstNodeCodeGenerator::visit(const NoExpr&) 
{ 
    
}


