#include "ast.hpp"

Program::Program(const std::vector<Class>& c)
    : classes(c)
{

}


Class::Class(const Symbol& cname, const Symbol& super, const Symbol& file, 
        const std::vector<Attribute>& attribs, const std::vector<Method>& funcs)
    : name(cname), parent(super), filename(file), attributes(attribs), methods(funcs)
{

}


Attribute::Attribute(const Symbol& aname, const Symbol& type, 
        const Expression& initexpr)
    : name(aname), type_decl(type), init(initexpr)
{

}


Method::Method(const Symbol& mname, const Symbol& ret, 
        const std::vector<Formal>& formals, const Expression& expr)
    : name(mname), return_type(ret), params(formals), body(expr)
{

}

Formal::Formal(const Symbol& fname, const Symbol& type)
    : name(fname), type_decl(type)
{

}

StringConst::StringConst(const Symbol& tok)
    : token(tok)
{

}

IntConst::IntConst(const Symbol& tok)
    : token(tok)
{

}

BoolConst::BoolConst(bool val)
    : value(val)
{

}

New::New(const Symbol& typ)
    : type(typ)
{

}

IsVoid::IsVoid(const Expression& pred)
    : expr(pred)
{

}

CaseBranch::CaseBranch(const Symbol& cname, const Symbol& type, 
        const Expression& exp)
    : name(cname), type_decl(type), expr(exp)
{

}

Assign::Assign(const Symbol& aname, const Expression& init)
    : name(aname), rhs(init)
{

}

Block::Block(const std::vector<Expression>& block)
    : body(block)
{

}

If::If(const Expression& pred, const Expression& truebr, 
        const Expression& falsebr)
    : predicate(pred), iftrue(truebr), iffalse(falsebr)
{

}

While::While(const Expression& pred, const Expression& bod)
    : predicate(pred), body(bod)
{

}

Complement::Complement(const Expression& e)
    : expr(e)
{

}

LessThan::LessThan(const Expression& l, const Expression& r)
    : lhs(l), rhs(r)
{

}

EqualTo::EqualTo(const Expression& l, const Expression& r)
    : lhs(l), rhs(r)
{

}

LessThanEqualTo::LessThanEqualTo(const Expression& l, const Expression& r)
    : lhs(l), rhs(r)
{

}

Plus::Plus(const Expression& l, const Expression& r)
    : lhs(l), rhs(r)
{

}


Sub::Sub(const Expression& l, const Expression& r)
    : lhs(l), rhs(r)
{

}


Mul::Mul(const Expression& l, const Expression& r)
    : lhs(l), rhs(r)
{

}

Div::Div(const Expression& l, const Expression& r)
    : lhs(l), rhs(r)
{

}

Not::Not(const Expression& rhs)
    : expr(rhs)
{

}

StaticDispatch::StaticDispatch(const Expression& objexpr, const Symbol& stype, 
        const Symbol& func, const Expression& act)
   : obj(objexpr), type(stype), method(func), actual(act)
{

}


DynamicDispatch::DynamicDispatch(const Expression& objexpr, 
        const Symbol& func, const Expression& act)
    : obj(objexpr), method(func), actual(act)
{

}

Let::Let(const Symbol& lname, const Symbol& type, 
        const Expression& initexpr, const Expression& bodyexpr)
    : name(lname), type_decl(type), init(initexpr), body(bodyexpr)
{

}

Case::Case(const Expression& exp, const std::vector<CaseBranch>& cb)
    : expr(exp), branches(cb)
{

}

Object::Object(const Symbol& obj)
    : name(obj)
{

}
