#include "ast.hpp"

Program::Program(const std::vector<std::shared_ptr<Class>>& c)
    : classes(c)
{

}

Class::Class(const Symbol& cname, const Symbol& super, const Symbol& file, 
        const std::vector<std::shared_ptr<Feature>>& f)
    : name(cname), parent(super), filename(file), features(f)
{

}


Attribute::Attribute(const Symbol& aname, const Symbol& type, 
        const std::shared_ptr<Expression>& initexpr)
    : name(aname), type_decl(type), init(initexpr)
{

}

Feature::feature_type Attribute::get_type() const
{
    return Feature::ATTRIBUTE;
}


Method::Method(const Symbol& mname, const Symbol& ret, 
        const std::vector<std::shared_ptr<Formal>>& formals, const std::shared_ptr<Expression>& expr)
    : name(mname), return_type(ret), params(formals), body(expr)
{

}

Feature::feature_type Method::get_type() const
{
    return Feature::METHOD;
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

IsVoid::IsVoid(const std::shared_ptr<Expression>& pred)
    : expr(pred)
{

}

CaseBranch::CaseBranch(const Symbol& cname, const Symbol& type, 
        const std::shared_ptr<Expression>& exp)
    : name(cname), type_decl(type), expr(exp)
{

}

Assign::Assign(const Symbol& aname, const std::shared_ptr<Expression>& init)
    : name(aname), rhs(init)
{

}

Block::Block(const std::vector<std::shared_ptr<Expression>>& block)
    : body(block)
{

}

If::If(const std::shared_ptr<Expression>& pred, const std::shared_ptr<Expression>& truebr, 
        const std::shared_ptr<Expression>& falsebr)
    : predicate(pred), iftrue(truebr), iffalse(falsebr)
{

}

While::While(const std::shared_ptr<Expression>& pred, const std::shared_ptr<Expression>& bod)
    : predicate(pred), body(bod)
{

}

Complement::Complement(const std::shared_ptr<Expression>& e)
    : expr(e)
{

}

LessThan::LessThan(const std::shared_ptr<Expression>& l, const std::shared_ptr<Expression>& r)
    : lhs(l), rhs(r)
{

}

EqualTo::EqualTo(const std::shared_ptr<Expression>& l, const std::shared_ptr<Expression>& r)
    : lhs(l), rhs(r)
{

}

LessThanEqualTo::LessThanEqualTo(const std::shared_ptr<Expression>& l, 
        const std::shared_ptr<Expression>& r)
    : lhs(l), rhs(r)
{

}

Plus::Plus(const std::shared_ptr<Expression>& l, const std::shared_ptr<Expression>& r)
    : lhs(l), rhs(r)
{

}


Sub::Sub(const std::shared_ptr<Expression>& l, const std::shared_ptr<Expression>& r)
    : lhs(l), rhs(r)
{

}


Mul::Mul(const std::shared_ptr<Expression>& l, const std::shared_ptr<Expression>& r)
    : lhs(l), rhs(r)
{

}

Div::Div(const std::shared_ptr<Expression>& l, const std::shared_ptr<Expression>& r)
    : lhs(l), rhs(r)
{

}

Not::Not(const std::shared_ptr<Expression>& rhs)
    : expr(rhs)
{

}

StaticDispatch::StaticDispatch(const std::shared_ptr<Expression>& objexpr, const Symbol& stype, 
        const Symbol& func, const std::vector<std::shared_ptr<Expression>>& act)
   : obj(objexpr), type(stype), method(func), actual(act)
{

}


DynamicDispatch::DynamicDispatch(const std::shared_ptr<Expression>& objexpr, 
        const Symbol& func, const std::vector<std::shared_ptr<Expression>>& act)
    : obj(objexpr), method(func), actual(act)
{

}

Let::Let(const Symbol& lname, const Symbol& type, const std::shared_ptr<Expression>& initexpr, 
        const std::shared_ptr<Expression>& bodyexpr)
    : name(lname), type_decl(type), init(initexpr), body(bodyexpr)
{

}

Case::Case(const std::shared_ptr<Expression>& exp, const std::vector<std::shared_ptr<CaseBranch>>& cb)
    : expr(exp), branches(cb)
{

}

Object::Object(const Symbol& obj)
    : name(obj)
{

}
