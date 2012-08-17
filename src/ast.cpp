#include "ast.hpp"
#include "tokentable.hpp"
#include "constants.hpp"

#include <iomanip>
#include <iostream>
#include <algorithm>
#include <functional>

using namespace constants;

void AstNode::setloc(std::size_t line, const std::string& file)
{
    line_no = line;
    filename = file;
}

Program::Program(const Classes& c)
    : classes(c)
{

}

void Program::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}

Class::Class(const Symbol& cname, const Symbol& super, const Attributes& attr, const Methods& funcs)
    : name(cname), parent(super), attributes(attr), methods(funcs)
{

}

void Class::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}


Attribute::Attribute(const Symbol& aname, const Symbol& type, 
        const ExpressionPtr& initexpr)
    : name(aname), type_decl(type), init(initexpr)
{

}

void Attribute::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}

Method::Method(const Symbol& mname, const Symbol& ret, 
        const Formals& formals, const ExpressionPtr& expr)
    : name(mname), return_type(ret), params(formals), body(expr)
{

}

void Method::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}

Formal::Formal(const Symbol& fname, const Symbol& type)
    : name(fname), type_decl(type)
{

}

void Formal::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}

StringConst::StringConst(const Symbol& tok)
    : token(tok)
{

}

void StringConst::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}

IntConst::IntConst(const Symbol& tok)
    : token(tok)
{

}

void IntConst::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}

BoolConst::BoolConst(bool val)
    : value(val)
{

}

void BoolConst::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}

New::New(const Symbol& typ)
    : type_decl(typ)
{

}

void New::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}

IsVoid::IsVoid(const ExpressionPtr& pred)
    : expr(pred)
{

}

void IsVoid::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}

CaseBranch::CaseBranch(const Symbol& cname, const Symbol& type, 
        const ExpressionPtr& exp)
    : name(cname), type_decl(type), expr(exp)
{

}

void CaseBranch::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}

Assign::Assign(const Symbol& aname, const ExpressionPtr& init)
    : name(aname), rhs(init)
{

}

void Assign::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}

Block::Block(const Expressions& block)
    : body(block)
{

}

void Block::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}

If::If(const ExpressionPtr& pred, const ExpressionPtr& truebr, 
        const ExpressionPtr& falsebr)
    : predicate(pred), iftrue(truebr), iffalse(falsebr)
{

}

void If::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}

While::While(const ExpressionPtr& pred, const ExpressionPtr& bod)
    : predicate(pred), body(bod)
{

}

void While::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}

Complement::Complement(const ExpressionPtr& e)
    : expr(e)
{

}

void Complement::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}

LessThan::LessThan(const ExpressionPtr& l, const ExpressionPtr& r)
    : lhs(l), rhs(r)
{

}

void LessThan::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}

EqualTo::EqualTo(const ExpressionPtr& l, const ExpressionPtr& r)
    : lhs(l), rhs(r)
{

}

void EqualTo::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}

LessThanEqualTo::LessThanEqualTo(const ExpressionPtr& l, 
        const ExpressionPtr& r)
    : lhs(l), rhs(r)
{

}

void LessThanEqualTo::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}

Plus::Plus(const ExpressionPtr& l, const ExpressionPtr& r)
    : lhs(l), rhs(r)
{

}

void Plus::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}

Sub::Sub(const ExpressionPtr& l, const ExpressionPtr& r)
    : lhs(l), rhs(r)
{

}

void Sub::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}

Mul::Mul(const ExpressionPtr& l, const ExpressionPtr& r)
    : lhs(l), rhs(r)
{

}

void Mul::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}

Div::Div(const ExpressionPtr& l, const ExpressionPtr& r)
    : lhs(l), rhs(r)
{

}

void Div::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}

Not::Not(const ExpressionPtr& rhs)
    : expr(rhs)
{

}

void Not::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}

StaticDispatch::StaticDispatch(const ExpressionPtr& objexpr, const Symbol& stype, 
        const Symbol& func, const Expressions& act)
   : obj(objexpr), type_decl(stype), method(func), actual(act)
{

}

void StaticDispatch::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}

DynamicDispatch::DynamicDispatch(const ExpressionPtr& objexpr, 
        const Symbol& func, const Expressions& act)
    : obj(objexpr), method(func), actual(act)
{

}

void DynamicDispatch::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}

Let::Let(const Symbol& lname, const Symbol& type, const ExpressionPtr& initexpr, 
        const ExpressionPtr& bodyexpr)
    : name(lname), type_decl(type), init(initexpr), body(bodyexpr)
{

}

void Let::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}

Case::Case(const ExpressionPtr& exp, const Cases& cb)
    : expr(exp), branches(cb)
{

}

void Case::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}

Object::Object(const Symbol& obj)
    : name(obj)
{

}

void Object::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}

NoExpr::NoExpr() 
{

}

void NoExpr::accept(AstNodeVisitor& visitor)
{
    visitor.visit(*this);
}
