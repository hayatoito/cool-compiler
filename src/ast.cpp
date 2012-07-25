#include "ast.hpp"
#include "tokentable.hpp"
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

Symbol Program::type_check(SemanticAnalyzer& sa, Environment& env) const
{

}

Symbol Class::type_check(SemanticAnalyzer& sa, Environment& env) const 
{

}

Symbol Attribute::type_check(SemanticAnalyzer& sa, Environment& env) const
{

}

Symbol Method::type_check(SemanticAnalyzer& sa, Environment& env) const
{

}

Symbol Formal::type_check(SemanticAnalyzer& sa, Environment& env) const
{

}

Symbol StringConst::type_check(SemanticAnalyzer& sa, Environment& env) const
{

}


Symbol IntConst::type_check(SemanticAnalyzer& sa, Environment& env) const
{

}

Symbol BoolConst::type_check(SemanticAnalyzer& sa, Environment& env) const
{

}

Symbol New::type_check(SemanticAnalyzer& sa, Environment& env) const
{
    
}

Symbol IsVoid::type_check(SemanticAnalyzer& sa, Environment& env) const
{

}

Symbol CaseBranch::type_check(SemanticAnalyzer& sa, Environment& env) const
{

}

Symbol Assign::type_check(SemanticAnalyzer& sa, Environment& env) const
{

}

Symbol Block::type_check(SemanticAnalyzer& sa, Environment& env) const
{

}

Symbol If::type_check(SemanticAnalyzer& sa, Environment& env) const
{

}

Symbol While::type_check(SemanticAnalyzer& sa, Environment& env) const
{

}

Symbol Complement::type_check(SemanticAnalyzer& sa, Environment& env) const
{

}

Symbol LessThan::type_check(SemanticAnalyzer& sa, Environment& env) const
{

}

Symbol EqualTo::type_check(SemanticAnalyzer& sa, Environment& env) const
{

}

Symbol LessThanEqualTo::type_check(SemanticAnalyzer& sa, Environment& env) const
{

}

Symbol Plus::type_check(SemanticAnalyzer& sa, Environment& env) const
{

}

Symbol Sub::type_check(SemanticAnalyzer& sa, Environment& env) const
{

}

Symbol Mul::type_check(SemanticAnalyzer& sa, Environment& env) const
{

}

Symbol Div::type_check(SemanticAnalyzer& sa, Environment& env) const
{

}

Symbol Not::type_check(SemanticAnalyzer& sa, Environment& env) const
{

}

Symbol StaticDispatch::type_check(SemanticAnalyzer& sa, Environment& env) const
{

}

Symbol DynamicDispatch::type_check(SemanticAnalyzer& sa, Environment& env) const
{

}

Symbol Let::type_check(SemanticAnalyzer& sa, Environment& env) const
{

}

Symbol Case::type_check(SemanticAnalyzer& sa, Environment& env) const
{

}

Symbol Object::type_check(SemanticAnalyzer& sa, Environment& env) const
{
    Symbol obj_type = env.symtbl.lookup(name);

    /*
    if (obj_type == NULL)
    {
        std::cerr << "Identifier not found!\n";
        obj_type = object;
    }
    */

    
}

Symbol NoExpr::type_check(SemanticAnalyzer& sa, Environment& env) const
{

}
