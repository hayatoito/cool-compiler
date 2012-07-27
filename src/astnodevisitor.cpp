#include "astnodevisitor.hpp"
#include "tokentable.hpp"
#include <memory>
#include <iomanip>
#include <cmath>
#include <sstream>

extern std::shared_ptr<Program> ast_root;

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

const Symbol AstNodeCodeGenerator::object = idtable().add("Object");
const Symbol AstNodeCodeGenerator::integer = idtable().add("Int");
const Symbol AstNodeCodeGenerator::boolean = idtable().add("Bool");
const Symbol AstNodeCodeGenerator::string = idtable().add("String");
const Symbol AstNodeCodeGenerator::io = idtable().add("IO");
const Symbol AstNodeCodeGenerator::self = idtable().add("self");
const Symbol AstNodeCodeGenerator::SELF_TYPE = idtable().add("SELF_TYPE");
const Symbol AstNodeCodeGenerator::noclass = idtable().add("NoClass");
const Symbol AstNodeCodeGenerator::notype = idtable().add("NoType");
const Symbol AstNodeCodeGenerator::prim_slot = idtable().add("prim_slot");
const Symbol AstNodeCodeGenerator::abort = idtable().add("abort");
const Symbol AstNodeCodeGenerator::type_name = idtable().add("type_name");
const Symbol AstNodeCodeGenerator::copy = idtable().add("copy");
const Symbol AstNodeCodeGenerator::main = idtable().add("main");
const Symbol AstNodeCodeGenerator::out_int = idtable().add("out_int");
const Symbol AstNodeCodeGenerator::in_int = idtable().add("in_int");
const Symbol AstNodeCodeGenerator::in_string = idtable().add("in_string");
const Symbol AstNodeCodeGenerator::out_string = idtable().add("out_string");
const Symbol AstNodeCodeGenerator::substr = idtable().add("substr");
const Symbol AstNodeCodeGenerator::length = idtable().add("length");
const Symbol AstNodeCodeGenerator::concat = idtable().add("concat");
const Symbol AstNodeCodeGenerator::arg = idtable().add("arg");
const Symbol AstNodeCodeGenerator::arg2 = idtable().add("arg2");
const Symbol AstNodeCodeGenerator::val = idtable().add("val");
const Symbol AstNodeCodeGenerator::str_field = idtable().add("str_field");

void AstNodeCodeGenerator::emit_addiu(const char* dst, const char* src1, int imm)
{
    os << "\taddiu\t$" << dst << ", $" << src1 << ", " << imm << "\n";
}

void AstNodeCodeGenerator::emit_add(const char* dst, const char* src1, const char* src2)
{
    os << "\tadd\t$" << dst << ", $" << src1 << ", $" << src2 << "\n";
}

void AstNodeCodeGenerator::emit_div(const char* dst, const char* src1, const char* src2)
{
    os << "\tdiv\t$" << dst << ", $" << src1 << ", $" << src2 << "\n";
}

void AstNodeCodeGenerator::emit_divu(const char* dst, const char* src1, const char* src2)
{
    os << "\tdivu\t$" << dst << ", $" << src1 << ", $" << src2 << "\n";
}

void AstNodeCodeGenerator::emit_mul(const char* dst, const char* src1, const char* src2)
{
    os << "\tmul\t$" << dst << ", $" << src1 << ", $" << src2 << "\n";
}

void AstNodeCodeGenerator::emit_sub(const char* dst, const char* src1, const char* src2)
{
    os << "\tsub\t$" << dst << ", $" << src1 << ", $" << src2 << "\n";
}

void AstNodeCodeGenerator::emit_and(const char* dst, const char* src1, const char* src2)
{
    os << "\tand\t$" << dst << ", $" << src1 << ", $" << src2 << "\n";
}

void AstNodeCodeGenerator::emit_neg(const char* dst, const char* src)
{
    os << "\tneg\t$" << dst << ", $" << src << "\n";
}

void AstNodeCodeGenerator::emit_nor(const char* dst, const char* src1, const char* src2)
{
    os << "\tnor\t$" << dst << ", $"  << src1 << ", $" << src2 << "\n";
}

void AstNodeCodeGenerator::emit_not(const char* dst, const char* src)
{
    os << "\tnot\t$" << dst << ", $" << src << "\n";
}

void AstNodeCodeGenerator::emit_or(const char* dst, const char* src1, const char* src2)
{
    os << "\tor\t$" << dst << ", $"  << src1 << ", $" << src2 << "\n";
}

void AstNodeCodeGenerator::emit_xor(const char* dst, const char* src1, const char* src2)
{
    os << "\txor\t$" << dst << ", $"  << src1 << ", $" << src2 << "\n";
}

void AstNodeCodeGenerator::emit_li(const char* dst, int imm)
{
    os << "\tli\t$" << dst << ", " << imm << "\n";
}

void AstNodeCodeGenerator::emit_lui(const char* dst, int imm)
{
    os << "\tlui\t$" << dst << ", " << imm << "\n";
}

void AstNodeCodeGenerator::emit_seq(const char* dst, const char* src1, const char* src2)
{
    os << "\tseq\t$" << dst << ", $" << src1 << ", $" << src2 << "\n";
}

void AstNodeCodeGenerator::emit_seq(const char* dst, const char* src1, int imm)
{
    os << "\tseq\t$" << dst << ", $" << src1 << ", " << imm << "\n";
}

void AstNodeCodeGenerator::emit_sge(const char* dst, const char* src1, const char* src2)
{
    os << "\tsge\t$" << dst << ", $" << src1 << ", $" << src2 << "\n";
}

void AstNodeCodeGenerator::emit_sge(const char* dst, const char* src1, int imm)
{
    os << "\tsge\t$" << dst << ", $" << src1 << ", " << imm << "\n";
}

void AstNodeCodeGenerator::emit_sgt(const char* dst, const char* src1, const char* src2)
{
    os << "\tsgt\t$" << dst << ", $" << src1 << ", $" << src2 << "\n";
}

void AstNodeCodeGenerator::emit_sgt(const char* dst, const char* src1, int imm)
{
    os << "\tsgt\t$" << dst << ", $" << src1 << ", " << imm << "\n";
}

void AstNodeCodeGenerator::emit_sle(const char* dst, const char* src1, const char* src2)
{
    os << "\tsle\t$" << dst << ", $" << src1 << ", $" << src2 << "\n";
}

void AstNodeCodeGenerator::emit_sle(const char* dst, const char* src1, int imm)
{
    os << "\tsle\t$" << dst << ", $" << src1 << ", " << imm << "\n";
}

void AstNodeCodeGenerator::emit_sne(const char* dst, const char* src1, const char* src2)
{
    os << "\tsne\t$" << dst << ", $" << src1 << ", $" << src2 << "\n";
}

void AstNodeCodeGenerator::emit_sne(const char* dst, const char* src1, int imm)
{
    os << "\tsne\t$" << dst << ", $" << src1 << ", " << imm << "\n";
}

void AstNodeCodeGenerator::emit_b(const char* label)
{
    os << "\tb\t" << label << "\n";
}

void AstNodeCodeGenerator::emit_beq(const char* src1, const char* src2, const char* label)
{
    os << "\tbeq\t$" << src1 << ", $" << src2 << ", " << label;
}

void AstNodeCodeGenerator::emit_beq(const char* src1, int imm, const char* label)
{
    os << "\tbeq\t$" << src1 << ", " << imm << ", " << label;
}

void AstNodeCodeGenerator::emit_bge(const char* src1, const char* src2, const char* label)
{
    os << "\tbge\t$" << src1 << ", $" << src2 << ", " << label;
}

void AstNodeCodeGenerator::emit_bge(const char* src1, int imm, const char* label)
{
    os << "\tbge\t$" << src1 << ", " << imm << ", " << label;
}

void AstNodeCodeGenerator::emit_bne(const char* src1, const char* src2, const char* label)
{
    os << "\tbne\t$" << src1 << ", $" << src2 << ", " << label;
}

void AstNodeCodeGenerator::emit_bne(const char* src1, int imm, const char* label)
{
    os << "\tbne\t$" << src1 << ", " << imm << ", " << label;
}

void AstNodeCodeGenerator::emit_j(const char* label)
{
    os << "\tj\t" << label << "\n";
}

void AstNodeCodeGenerator::emit_jal(const char* label)
{
    os << "\tjal\t" << label << "\n";
}

void AstNodeCodeGenerator::emit_jalr(const char* src)
{
    os << "\tjalr\t$" << src << "\n";
}

void AstNodeCodeGenerator::emit_jr(const char* src)
{
    os << "\tjr\t$" << src << "\n";
}

void AstNodeCodeGenerator::emit_la(const char* dst, const char* addr)
{
    os << "\tla\t$" << dst << ", " << addr << "\n";
}

void AstNodeCodeGenerator::emit_lb(const char* dst, const char* addr)
{
    os << "\tlb\t$" << dst << ", " << addr << "\n";
}

void AstNodeCodeGenerator::emit_ld(const char* dst, const char* addr)
{
    os << "\tld\t$" << dst << ", " << addr << "\n";
}

void AstNodeCodeGenerator::emit_lw(const char* dst, const char* addr)
{
    os << "\tlw\t$" << dst << ", " << addr << "\n";
}

void AstNodeCodeGenerator::emit_sb(const char* dst, const char* addr)
{
    os << "\tsb\t$" << dst << ", " << addr << "\n";
}

void AstNodeCodeGenerator::emit_sd(const char* dst, const char* addr)
{
    os << "\tsd\t$" << dst << ", " << addr << "\n";
}

void AstNodeCodeGenerator::emit_sw(const char* dst, const char* addr)
{
    os << "\tsw\t$" << dst << ", " << addr << "\n";
}

void AstNodeCodeGenerator::emit_move(const char* dst, const char* src)
{
    os << "\tmove\t$" << dst << ", $" << src << "\n";
}

void AstNodeCodeGenerator::emit_syscall()
{
    os << "\tsyscall\n";
}

void AstNodeCodeGenerator::emit_nop()
{
    os << "\tnop\n";
}

void AstNodeCodeGenerator::emit_align(int boundary)
{
    os << "\t.align\t" << boundary << "\n";
}

void AstNodeCodeGenerator::emit_ascii(const char* str)
{
    os << "\t.ascii\t\"" << str << "\"\n";
}

void AstNodeCodeGenerator::emit_asciiz(const char* str)
{
    os << "\t.asciiz\t\"" << str << "\"\n";
}

void AstNodeCodeGenerator::emit_byte(int val)
{
    os << "\t.byte\t" << val << "\n";
}

void AstNodeCodeGenerator::emit_globl(const char* sym)
{
    os << "\t.globl\t" << sym << "\n";
}

void AstNodeCodeGenerator::emit_word(int val)
{
    os << "\t.word\t" << val << "\n";
}

void AstNodeCodeGenerator::emit_word(const std::string& val)
{
    os << "\t.word\t" << val << "\n";
}

void AstNodeCodeGenerator::emit_word(const char* val)
{
    os << "\t.word\t" << val << "\n";
}

void AstNodeCodeGenerator::emit_label(const char* label)
{
    os << label << ":" << "\n";
}

void AstNodeCodeGenerator::emit_label(const std::string& label)
{
    os << label << ":" << "\n";
}

void AstNodeCodeGenerator::install_basic()
{
    Features object_features = { 
        std::make_shared<Method>(abort, object, Formals(), std::make_shared<NoExpr>()),
        std::make_shared<Method>(type_name, string, Formals(), std::make_shared<NoExpr>()),
        std::make_shared<Method>(copy, SELF_TYPE, Formals(), std::make_shared<NoExpr>())
    };

    ast_root->classes.push_back(std::make_shared<Class>(object, noclass, idtable().add("filename"), object_features));

    Formals io_formal1 = { std::make_shared<Formal>(arg, string) };
    Formals io_formal2 = { std::make_shared<Formal>(arg, integer) };
    Features io_features = {
        std::make_shared<Method>(out_string, SELF_TYPE, io_formal1, std::make_shared<NoExpr>()),
        std::make_shared<Method>(out_int, SELF_TYPE, io_formal2, std::make_shared<NoExpr>()),
        std::make_shared<Method>(in_string, string, Formals(), std::make_shared<NoExpr>()),
        std::make_shared<Method>(in_int, integer, Formals(), std::make_shared<NoExpr>())
    };

    ast_root->classes.push_back(std::make_shared<Class>(io, object, idtable().add("filename"), io_features));

    Features int_features = {
        std::make_shared<Attribute>(val, prim_slot, std::make_shared<NoExpr>())
    };

    ast_root->classes.push_back(std::make_shared<Class>(integer, object, idtable().add("filename"), int_features));

    Features bool_features = {
        std::make_shared<Attribute>(val, prim_slot, std::make_shared<NoExpr>())
    };

    ast_root->classes.push_back(std::make_shared<Class>(boolean, object, idtable().add("filename"), bool_features));

    Formals string_formal1 = { std::make_shared<Formal>(arg, string) };
    Formals string_formal2 = { 
        std::make_shared<Formal>(arg, integer),
        std::make_shared<Formal>(arg2, integer)
    };

    Features string_features = {
        std::make_shared<Attribute>(val, integer, std::make_shared<NoExpr>()),
        std::make_shared<Attribute>(str_field, prim_slot, std::make_shared<NoExpr>()),    
        std::make_shared<Method>(length, integer, Formals(), std::make_shared<NoExpr>()),
        std::make_shared<Method>(concat, string, string_formal1, std::make_shared<NoExpr>()),
        std::make_shared<Method>(substr, string, string_formal2, std::make_shared<NoExpr>())
    };

    ast_root->classes.push_back(std::make_shared<Class>(string, object, idtable().add("filename"), string_features));

    //Add basic classes to string table so a string constant 
    //will be created for them
    stringtable().add("Object");
    stringtable().add("Bool");
    stringtable().add("IO");
    stringtable().add("Int");
    stringtable().add("String");

    //Add basic classes to inheritance graph
    inherit_graph["Object"] = "NoClass";
    inherit_graph["Bool"] = "Object";
    inherit_graph["IO"] = "Object";
    inherit_graph["Int"] = "Object";
    inherit_graph["String"] = "Object";
}

void AstNodeCodeGenerator::code_constants() 
{
    //Add all class names to the string table so string constants 
    //will be created for them
    for (auto it = begin(inherit_graph); it != end(inherit_graph); ++it)
        stringtable().add(it->first);

    //emit assembly for string constants
    auto str_consts = stringtable().get_elems();

    for (auto it = begin(str_consts); it != end(str_consts); ++it)
    {
        os << "str_const";
        emit_label(std::to_string(stringtable().get_idx(it->first)).c_str());
        emit_word(STR_CLASS_TAG);
        emit_word(STR_CONST_BASE + ceil(it->first.size() / 4.0));
        emit_word("String_disptable");
        emit_word(it->first.size());
        emit_asciiz(it->first.c_str());
        emit_align(2); // align to word boundary (2^2)
    }

    //emit assembly for int constants
    auto int_consts = inttable().get_elems();

    for (auto it = begin(int_consts); it != end(int_consts); ++it)
    {
        os << "int_const";
        emit_label(std::to_string(inttable().get_idx(it->first)).c_str());
        emit_word(INT_CLASS_TAG);
        emit_word(INT_CONST_SIZE);
        emit_word("Int_disptable");

        int repr;
        std::stringstream(it->first) >> repr;
        emit_word(repr);
    }

    //emit assembly for bool constants
    emit_label("bool_const0"); // for false
    emit_word(BOOL_CLASS_TAG);
    emit_word(BOOL_CONST_SIZE);
    emit_word("Bool_disptable");
    emit_word(0);
 
    emit_label("bool_const1"); // for true
    emit_word(BOOL_CLASS_TAG);
    emit_word(BOOL_CONST_SIZE);
    emit_word("Bool_disptable");
    emit_word(1);
}

void AstNodeCodeGenerator::code_class_name_table()
{
    emit_label("class_name_table");

    for (auto it = begin(inherit_graph); it != end(inherit_graph); ++it)
    {
        std::ostringstream oss;
        oss << "str_const" << stringtable().get_idx(it->first);
        emit_word(oss.str().c_str());
    }
}

void AstNodeCodeGenerator::code_prototype_table()
{
    emit_label("class_prototype_table");

    for (auto it = begin(inherit_graph); it != end(inherit_graph); ++it)
    {
        emit_word(it->first + "_prototype");
        emit_word(it->first + "_init");
    }
}

void AstNodeCodeGenerator::code_dispatch_table(const std::string& class_node)
{
    if (class_node == "NoClass")
        return;

    code_dispatch_table(inherit_graph[class_node]);

    //ugly stuff here, have to fix this
    //two issues: 1. inheritance graph only contains class names as strings
    //            2. a dynamic cast has to be done to access method or attribute names
    for (auto it = begin(ast_root->classes); it != end(ast_root->classes); ++it)
    {
        if ((*it)->name == class_node)
        {
            for (auto fit = begin((*it)->features); fit != end((*it)->features); ++fit)
            {
                if ((*fit)->get_type() == Feature::METHOD)
                {
                    std::shared_ptr<Method> mptr(std::dynamic_pointer_cast<Method>(*fit));
                    emit_word(class_node + "_" + mptr->name.get_val());
                }
            }
        }
    }
}

std::map<std::string, int> AstNodeCodeGenerator::count_attrs()
{
    std::map<std::string, int> attr_count;

    for (auto it = begin(ast_root->classes); it != end(ast_root->classes); ++it)
    {
        attr_count[(*it)->name.get_val()] = 0;
        for (auto fit = begin((*it)->features); fit != end((*it)->features); ++fit)
        {
            if ((*fit)->get_type() == Feature::ATTRIBUTE)
                attr_count[(*it)->name.get_val()]++;
        }
    }

    return attr_count;
}

int AstNodeCodeGenerator::calc_obj_size(std::map<std::string, int>& attr_count, const std::string& class_name)
{
    int total = 0;
    std::string curr_class = class_name;

    while (curr_class != "NoClass")
    {
        total += attr_count[curr_class];
        curr_class = inherit_graph[curr_class];
    }

    return total;
}

void AstNodeCodeGenerator::emit_obj_attribs(const std::string& class_name)
{
    if (class_name == "NoClass")
        return;

    emit_obj_attribs(inherit_graph[class_name]);

    for (auto it = begin(ast_root->classes); it != end(ast_root->classes); ++it)
    {
        if ((*it)->name == class_name)
        {
            for (auto fit = begin((*it)->features); fit != end((*it)->features); ++fit)
            {
                if ((*fit)->get_type() == Feature::ATTRIBUTE)
                {
                    std::shared_ptr<Attribute> aptr(std::dynamic_pointer_cast<Attribute>(*fit));
                    emit_word(0);
                }
            }
        }
    }
}

void AstNodeCodeGenerator::code_prototype_objects()
{
    int classtag = 1;
    std::map<std::string, int> attr_count = count_attrs();

    for (auto it = begin(inherit_graph); it != end(inherit_graph); ++it)
    {
        emit_label(it->first + "_prototype");

        if (it->first == "String")
            emit_word(STR_CLASS_TAG);
        else if (it->first == "Int")
            emit_word(INT_CLASS_TAG);
        else if (it->first == "Bool")
            emit_word(BOOL_CLASS_TAG);
        else
            emit_word(classtag++);

        emit_word(OBJECT_HEADER_SIZE + calc_obj_size(attr_count, it->first));
        emit_word(it->first + "_disptable");
        emit_obj_attribs(it->first);
    }
}

void AstNodeCodeGenerator::visit(const Program& prog)
{
    os << ".data\n" 
       << "\t.align\t2\n"
       << "\t.globl\tclass_name_table\n"
       << "\t.globl\tMain_prototype\n"
       << "\t.globl\tInt_prototype\n"
       << "\t.globl\tString_prototype\n"
       << "\t.globl\tbool_const0\n"
       << "\t.globl\tbool_const1\n"
       << "\t.globl\t_int_tag\n"
       << "\t.globl\t_bool_tag\n"
       << "\t.globl\t_string_tag\n"
       << "_int_tag:\n"
       << "\t.word\t" << INT_CLASS_TAG << "\n"
       << "_bool_tag:\n"
       << "\t.word\t" << BOOL_CLASS_TAG << "\n"
       << "_string_tag:\n"
       << "\t.word\t" << STR_CLASS_TAG << "\n";

    code_constants();
    code_class_name_table();
    code_prototype_table();
    
    for (auto it = begin(inherit_graph); it != end(inherit_graph); ++it)
    {
        emit_label(it->first + "_disptable");
        code_dispatch_table(it->first);
    }

    code_prototype_objects();

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


