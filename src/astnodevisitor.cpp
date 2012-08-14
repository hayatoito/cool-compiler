#include "astnodevisitor.hpp"
#include "tokentable.hpp"

#include <memory>
#include <iomanip>

AstNodeDisplayer::AstNodeDisplayer(std::ostream& stream)
    : os(stream), depth(0)
{

}

void AstNodeDisplayer::visit(Program& prog)
{
    for (auto& cs : prog.classes)
        cs->accept(*this);
}

void AstNodeDisplayer::visit(Class& cs)
{
    os << std::setw(depth++ * 2) << "";
    os << "_class (" << cs.name << ")\n";  
    
    for (auto& feature : cs.features)
        feature->accept(*this);

    --depth;
}

void AstNodeDisplayer::visit(Attribute& attr)
{
    os << std::setw(depth++ * 2) << "";
    os << "_attribute (" << attr.name << ")\n";
    attr.init->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(Feature& feature)
{
    feature.accept(*this);
}

void AstNodeDisplayer::visit(Formal& formal) 
{ 
    os << std::setw(depth * 2) << "";
    os << "_formal (" << formal.name << ")\n";
}

void AstNodeDisplayer::visit(Method& method) 
{ 
    os << std::setw(depth++ * 2) << "";
    os << "_method (" << method.name << ")\n";

    for (auto& formal : method.params)
        formal->accept(*this); 

    method.body->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(StringConst& str) 
{ 
    os << std::setw(depth * 2) << "";
    os << "_stringconst(" << str.token << ") : " << str.type << "\n";
}

void AstNodeDisplayer::visit(IntConst& int_const) 
{
    os << std::setw(depth * 2) << "";
    os << "_intconst(" << int_const.token << ") : " << int_const.type << "\n";
}

void AstNodeDisplayer::visit(BoolConst& bool_const) 
{ 
    os << std::setw(depth * 2) << "";
    os << "_boolconst(" << bool_const.value << ") : " << bool_const.type << "\n";
}

void AstNodeDisplayer::visit(New& new_node) 
{
    os << std::setw(depth * 2) << "";
    os << "_new (" << new_node.type_decl << ") : " << new_node.type << "\n";
}

void AstNodeDisplayer::visit(IsVoid& isvoid) 
{ 
    os << std::setw(depth++ * 2) << "";
    os << "_isvoid : " << isvoid.type << "\n";
    isvoid.expr->accept(*this); 
    --depth;
}

void AstNodeDisplayer::visit(CaseBranch& branch) 
{ 
    os << std::setw(depth++ * 2) << "";
    os << "_casebranch (" << branch.name << ") : " << branch.type << "\n";
    branch.expr->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(Assign& assign) 
{ 
    os << std::setw(depth++ * 2) << "";
    os << "_assign (" << assign.name << ") : " << assign.type << "\n";
    assign.rhs->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(Block& block) 
{ 
    os << std::setw(depth++ * 2) << "";
    os << "_block : " << block.type << "\n";

    for (auto& expr : block.body)
        expr->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(If& ifstmt) 
{ 
    os << std::setw(depth++ * 2) << "";
    os << "_if : " << ifstmt.type << "\n";

    ifstmt.predicate->accept(*this);
    ifstmt.iftrue->accept(*this);
    ifstmt.iffalse->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(While& whilestmt) 
{ 
    os << std::setw(depth++ * 2) << "";
    os << "_while : " << whilestmt.type << "\n";

    whilestmt.predicate->accept(*this);
    whilestmt.body->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(Complement& comp) 
{ 
    os << std::setw(depth++ * 2) << "";
    os << "_complement : " << comp.type << "\n";
    comp.expr->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(LessThan& lt) 
{ 
    os << std::setw(depth++ * 2) << "";
    os << "_lessthan : " << lt.type << "\n";
    lt.lhs->accept(*this);
    lt.rhs->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(EqualTo& eq) 
{ 
    os << std::setw(depth++ * 2) << "";
    os << "_equalto : " << eq.type << "\n";
    eq.lhs->accept(*this);
    eq.rhs->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(LessThanEqualTo& lteq) 
{ 
    os << std::setw(depth++ * 2) << "";
    os << "_lessthanequalto : " << lteq.type << "\n";
    lteq.lhs->accept(*this);
    lteq.rhs->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(Plus& plus) 
{
    os << std::setw(depth++ * 2) << "";
    os << "_plus : " << plus.type << "\n";
    plus.lhs->accept(*this);
    plus.rhs->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(Sub& sub) 
{ 
    os << std::setw(depth++ * 2) << "";
    os << "_sub : " << sub.type << "\n";
    sub.lhs->accept(*this);
    sub.rhs->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(Mul& mul) 
{ 
    os << std::setw(depth++ * 2) << "";
    os << "_mul : " << mul.type << "\n";
    mul.lhs->accept(*this);
    mul.rhs->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(Div& div) 
{ 
    os << std::setw(depth++ * 2) << "";
    os << "_div : " << div.type << "\n";
    div.lhs->accept(*this);
    div.rhs->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(Not& nt) 
{ 
    os << std::setw(depth++ * 2) << "";
    os << "_not : " << nt.type << "\n";
    nt.expr->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(StaticDispatch& sdisp) 
{ 
    os << std::setw(depth++ * 2) << "";
    os << "_staticdispatch (" << sdisp.method << ") : " << sdisp.type << "\n";

    sdisp.obj->accept(*this);
    for (auto& e : sdisp.actual)
       e->accept(*this); 
    --depth;
}

void AstNodeDisplayer::visit(DynamicDispatch& ddisp) 
{ 
    os << std::setw(depth++ * 2) << "";
    os << "_dynamicdispatch (" << ddisp.method << ") : " << ddisp.type << "\n";

    ddisp.obj->accept(*this);
    for (auto& e : ddisp.actual)
       e->accept(*this); 
    --depth;
}

void AstNodeDisplayer::visit(Let& let) 
{ 
    os << std::setw(depth++ * 2) << "";
    os << "_let (" << let.name << ") : " << let.type << "\n";
    
    let.init->accept(*this);
    let.body->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(Case& caze) 
{ 
    os << std::setw(depth++ * 2) << "";
    os << "_case : " << caze.type << "\n";

    caze.expr->accept(*this);
    for (auto& br : caze.branches)
        br->accept(*this);
    --depth;
}

void AstNodeDisplayer::visit(Object& obj) 
{ 
    os << std::setw(depth * 2) << "";
    os << "_object (" << obj.name << ") : " << obj.type << "\n";
}

void AstNodeDisplayer::visit(NoExpr& ne) 
{ 
    os << std::setw(depth * 2) << "";
    os << "_noexpr : " << ne.type << "\n";
}
