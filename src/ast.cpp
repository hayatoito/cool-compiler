#include "ast.hpp"
#include <iomanip>
#include <iostream>

Program::Program(const std::vector<std::shared_ptr<Class>>& c)
    : classes(c)
{

}

void Program::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

Class::Class(const Symbol& cname, const Symbol& super, const Symbol& file, 
        const std::vector<std::shared_ptr<Feature>>& f)
    : name(cname), parent(super), filename(file), features(f)
{

}

void Class::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}


Attribute::Attribute(const Symbol& aname, const Symbol& type, 
        const std::shared_ptr<Expression>& initexpr)
    : name(aname), type_decl(type), init(initexpr)
{

}

void Attribute::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

Feature::feature_type Attribute::get_type() const
{
    return Feature::ATTRIBUTE;
}

void Feature::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

Method::Method(const Symbol& mname, const Symbol& ret, 
        const std::vector<std::shared_ptr<Formal>>& formals, const std::shared_ptr<Expression>& expr)
    : name(mname), return_type(ret), params(formals), body(expr)
{

}

void Method::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

Feature::feature_type Method::get_type() const
{
    return Feature::METHOD;
}

Formal::Formal(const Symbol& fname, const Symbol& type)
    : name(fname), type_decl(type)
{

}

void Formal::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

StringConst::StringConst(const Symbol& tok)
    : token(tok)
{

}

void StringConst::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

IntConst::IntConst(const Symbol& tok)
    : token(tok)
{

}

void IntConst::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

BoolConst::BoolConst(bool val)
    : value(val)
{

}

void BoolConst::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

New::New(const Symbol& typ)
    : type(typ)
{

}

void New::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

IsVoid::IsVoid(const std::shared_ptr<Expression>& pred)
    : expr(pred)
{

}

void IsVoid::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

CaseBranch::CaseBranch(const Symbol& cname, const Symbol& type, 
        const std::shared_ptr<Expression>& exp)
    : name(cname), type_decl(type), expr(exp)
{

}

void CaseBranch::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

Assign::Assign(const Symbol& aname, const std::shared_ptr<Expression>& init)
    : name(aname), rhs(init)
{

}

void Assign::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

Block::Block(const std::vector<std::shared_ptr<Expression>>& block)
    : body(block)
{

}

void Block::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

If::If(const std::shared_ptr<Expression>& pred, const std::shared_ptr<Expression>& truebr, 
        const std::shared_ptr<Expression>& falsebr)
    : predicate(pred), iftrue(truebr), iffalse(falsebr)
{

}

void If::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

While::While(const std::shared_ptr<Expression>& pred, const std::shared_ptr<Expression>& bod)
    : predicate(pred), body(bod)
{

}

void While::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

Complement::Complement(const std::shared_ptr<Expression>& e)
    : expr(e)
{

}

void Complement::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

LessThan::LessThan(const std::shared_ptr<Expression>& l, const std::shared_ptr<Expression>& r)
    : lhs(l), rhs(r)
{

}

void LessThan::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

EqualTo::EqualTo(const std::shared_ptr<Expression>& l, const std::shared_ptr<Expression>& r)
    : lhs(l), rhs(r)
{

}

void EqualTo::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

LessThanEqualTo::LessThanEqualTo(const std::shared_ptr<Expression>& l, 
        const std::shared_ptr<Expression>& r)
    : lhs(l), rhs(r)
{

}

void LessThanEqualTo::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

Plus::Plus(const std::shared_ptr<Expression>& l, const std::shared_ptr<Expression>& r)
    : lhs(l), rhs(r)
{

}

void Plus::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

Sub::Sub(const std::shared_ptr<Expression>& l, const std::shared_ptr<Expression>& r)
    : lhs(l), rhs(r)
{

}

void Sub::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

Mul::Mul(const std::shared_ptr<Expression>& l, const std::shared_ptr<Expression>& r)
    : lhs(l), rhs(r)
{

}

void Mul::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

Div::Div(const std::shared_ptr<Expression>& l, const std::shared_ptr<Expression>& r)
    : lhs(l), rhs(r)
{

}

void Div::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

Not::Not(const std::shared_ptr<Expression>& rhs)
    : expr(rhs)
{

}

void Not::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

StaticDispatch::StaticDispatch(const std::shared_ptr<Expression>& objexpr, const Symbol& stype, 
        const Symbol& func, const std::vector<std::shared_ptr<Expression>>& act)
   : obj(objexpr), type(stype), method(func), actual(act)
{

}

void StaticDispatch::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

DynamicDispatch::DynamicDispatch(const std::shared_ptr<Expression>& objexpr, 
        const Symbol& func, const std::vector<std::shared_ptr<Expression>>& act)
    : obj(objexpr), method(func), actual(act)
{

}

void DynamicDispatch::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

Let::Let(const Symbol& lname, const Symbol& type, const std::shared_ptr<Expression>& initexpr, 
        const std::shared_ptr<Expression>& bodyexpr)
    : name(lname), type_decl(type), init(initexpr), body(bodyexpr)
{

}

void Let::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

Case::Case(const std::shared_ptr<Expression>& exp, const std::vector<std::shared_ptr<CaseBranch>>& cb)
    : expr(exp), branches(cb)
{

}

void Case::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

Object::Object(const Symbol& obj)
    : name(obj)
{

}

void Object::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

NoExpr::NoExpr() 
{

}

void NoExpr::accept(AstNodeVisitor& visitor) const
{
    visitor.visit(*this);
}


// Visitor Implementation

AstNodePrintVisitor::AstNodePrintVisitor(std::ostream& stream)
    : os(stream), depth(0)
{

}

void AstNodePrintVisitor::visit(const Program& prog)
{
    for (auto cs : prog.classes)
        cs->accept(*this);
}

void AstNodePrintVisitor::visit(const Class& cs)
{
    os << std::setw(depth++ * 2) << "-class (" << cs.name << ")\n";  
    
    for (auto feature : cs.features)
        feature->accept(*this);
}

void AstNodePrintVisitor::visit(const Attribute& attr)
{
    os< << std::setw(depth++ * 2) << "-attribute (" << attr.name << ")\n";

    f
}

void AstNodePrintVisitor::visit(const Feature&)
{

}

void AstNodePrintVisitor::visit(const Formal&) { }
void AstNodePrintVisitor::visit(const Method&) { }
void AstNodePrintVisitor::visit(const StringConst&) { }
void AstNodePrintVisitor::visit(const IntConst&) { }
void AstNodePrintVisitor::visit(const BoolConst&) { }
void AstNodePrintVisitor::visit(const New&) { }
void AstNodePrintVisitor::visit(const IsVoid&) { }
void AstNodePrintVisitor::visit(const CaseBranch&) { }
void AstNodePrintVisitor::visit(const Assign&) { }
void AstNodePrintVisitor::visit(const Block&) { }
void AstNodePrintVisitor::visit(const If&) { }
void AstNodePrintVisitor::visit(const While&) { }
void AstNodePrintVisitor::visit(const Complement&) { }
void AstNodePrintVisitor::visit(const LessThan&) { }
void AstNodePrintVisitor::visit(const EqualTo&) { }
void AstNodePrintVisitor::visit(const LessThanEqualTo&) { }
void AstNodePrintVisitor::visit(const Plus&) { }
void AstNodePrintVisitor::visit(const Sub&) { }
void AstNodePrintVisitor::visit(const Mul&) { }
void AstNodePrintVisitor::visit(const Div&) { }
void AstNodePrintVisitor::visit(const Not&) { }
void AstNodePrintVisitor::visit(const StaticDispatch&) { }
void AstNodePrintVisitor::visit(const DynamicDispatch&) { }
void AstNodePrintVisitor::visit(const Let&) { }
void AstNodePrintVisitor::visit(const Case&) { }
void AstNodePrintVisitor::visit(const Object&) { }
void AstNodePrintVisitor::visit(const NoExpr&) { }

