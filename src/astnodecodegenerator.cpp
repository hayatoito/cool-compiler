#include "astnodecodegenerator.hpp"
#include "utility.hpp"
#include "constants.hpp"

#include <cmath>
#include <sstream>
#include <stack>

// defined in main.cpp
extern ProgramPtr ast_root;

using namespace constants;

AstNodeCodeGenerator::AstNodeCodeGenerator(const std::map<ClassPtr, ClassPtr>& ig,
        std::ostream& stream)
    : inherit_graph(ig), os(stream), curr_attr_count(0), while_count(0), if_count(0)
{

}

void AstNodeCodeGenerator::emit_addiu(const char* dst, const char* src1, int imm)
{
    os << "\taddiu\t$" << dst << ", $" << src1 << ", " << imm << "\n";
}

void AstNodeCodeGenerator::emit_addi(const char* dst, const char* src1, int imm)
{
    os << "\taddi\t$" << dst << ", $" << src1 << ", " << imm << "\n";
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

void AstNodeCodeGenerator::emit_b(const std::string& label)
{
    os << "\tb\t" << label << "\n";
}

void AstNodeCodeGenerator::emit_beq(const char* src1, const char* src2, const std::string& label)
{
    os << "\tbeq\t$" << src1 << ", $" << src2 << ", " << label << "\n";
}

void AstNodeCodeGenerator::emit_beq(const char* src1, int imm, const std::string& label)
{
    os << "\tbeq\t$" << src1 << ", " << imm << ", " << label << "\n";
}

void AstNodeCodeGenerator::emit_bge(const char* src1, const char* src2, const std::string& label)
{
    os << "\tbge\t$" << src1 << ", $" << src2 << ", " << label << "\n";
}

void AstNodeCodeGenerator::emit_bge(const char* src1, int imm, const std::string& label)
{
    os << "\tbge\t$" << src1 << ", " << imm << ", " << label << "\n";
}

void AstNodeCodeGenerator::emit_bne(const char* src1, const char* src2, const std::string& label)
{
    os << "\tbne\t$" << src1 << ", $" << src2 << ", " << label << "\n";
}

void AstNodeCodeGenerator::emit_bne(const char* src1, int imm, const std::string& label)
{
    os << "\tbne\t$" << src1 << ", " << imm << ", " << label << "\n";
}

void AstNodeCodeGenerator::emit_j(const std::string& label)
{
    os << "\tj\t" << label << "\n";
}

void AstNodeCodeGenerator::emit_jal(const std::string& label)
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

void AstNodeCodeGenerator::emit_la(const char* dst, const std::string& addr)
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

void AstNodeCodeGenerator::emit_lw(const char* dst, int offset, const char* src)
{
    os << "\tlw\t$" << dst << ", " << offset << "($" << src << ")\n";
}

void AstNodeCodeGenerator::emit_sb(const char* dst, const char* addr)
{
    os << "\tsb\t$" << dst << ", " << addr << "\n";
}

void AstNodeCodeGenerator::emit_sd(const char* dst, const char* addr)
{
    os << "\tsd\t$" << dst << ", " << addr << "\n";
}

void AstNodeCodeGenerator::emit_sw(const char* dst, int offset, const char* src)
{
    os << "\tsw\t$" << dst << ", " << offset << "($" << src << ")\n";
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

void AstNodeCodeGenerator::emit_ascii(const std::string& str)
{
    os << "\t.ascii\t\"" << str << "\"\n";
}

void AstNodeCodeGenerator::emit_asciiz(const std::string& str)
{
    os << "\t.asciiz\t\"" << str << "\"\n";
}

void AstNodeCodeGenerator::emit_byte(int val)
{
    os << "\t.byte\t" << val << "\n";
}

void AstNodeCodeGenerator::emit_globl(const std::string& sym)
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

void AstNodeCodeGenerator::emit_label(const std::string& label)
{
    os << label << ":" << "\n";
}

void AstNodeCodeGenerator::emit_push(int num_words)
{
    emit_addiu("sp", "sp", WORD_SIZE * -num_words);
}

void AstNodeCodeGenerator::emit_pop(int num_bytes)
{
    emit_addiu("sp", "sp", WORD_SIZE * num_bytes);
}

void AstNodeCodeGenerator::code_constants()
{
    // Add all class names to the string table so string constants
    // will be created for them (for class name table code gen)
    for (auto& p : inherit_graph)
        stringtable().add(p.first->name.get_val());

    auto str_consts = stringtable().get_elems();

    stringtable().add(""); // add empty string to string const table as this is the
                           // default value of a newly allocated string object

    for (auto it = begin(str_consts); it != end(str_consts); ++it)
    {
        os << "str_const";
        emit_label(std::to_string(stringtable().get_idx(it->first)).c_str());
        emit_word(STR_CLASS_TAG);

        // total size of string constant is 4 + number of words required to store the characters
        emit_word(STR_CONST_BASE + ceil(it->first.size() / 4.0));
        emit_word("String_disptable");
        emit_word(it->first.size());
        emit_asciiz(it->first.c_str());
        emit_align(2); // align to word boundary (2^2)
    }

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

    // code gen for boolean constants true and false
    // a boolean is internally represented as an object
    // with one integer attribute (0 for false and 1 for true)
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

/*
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
*/

void AstNodeCodeGenerator::code_dispatch_table(const ClassPtr& class_node)
{
    std::map<Symbol, Symbol> mnames; // map of all method names
    std::stack<ClassPtr> recur; // the methods in the dispatch table must be ordered
                                // starting from the top of the hierarchy down to the class
                                // class_node is pointing to

    ClassPtr cptr = class_node;

    // go up the inheritance tree and for each class, push it to the stack (so class Object
    // will be on the top of the stack after this loop) and add all the method names in mnames.
    // this is used to check later if a method was overriden
    while (cptr->name != NOCLASS)
    {
        recur.push(cptr);

        for (auto& method : cptr->methods)
        {
            if (mnames.find(method->name) == end(mnames))
                mnames[method->name] = cptr->name;
        }

        cptr = inherit_graph[cptr];
    }

    std::size_t dispoffset = 0;

    // go through the stack (goes down the inheritance tree, starting from Object)
    while (!recur.empty())
    {
        ClassPtr head = recur.top();

        // for each method in the current class, if the method is still in the mnames
        // table (not overriden), add it to the method table with the offset then remove
        // the method name from mnames so there won't be any duplication if a method
        // is overriden by a derived class
        for (auto& method : head->methods)
        {
            if (mnames.find(method->name) != end(mnames))
            {
                method_tbl[class_node->name][method->name] = dispoffset++;
                emit_word(mnames[method->name].get_val() + "." + method->name.get_val());
                mnames.erase(method->name);
            }
        }

        recur.pop();
    }
}

int AstNodeCodeGenerator::calc_obj_size(const ClassPtr& class_node)
{
    int total = 0;
    ClassPtr curr_class = class_node;

    while (curr_class->name != NOCLASS)
    {
        total += curr_class->attributes.size();
        curr_class = inherit_graph[curr_class];
    }

    return total;
}

void AstNodeCodeGenerator::emit_obj_attribs(const ClassPtr& class_node)
{
    if (class_node->name == NOCLASS)
        return;

    emit_obj_attribs(inherit_graph[class_node]);

    for (auto& attrib : class_node->attributes)
        emit_word(0);
}

void AstNodeCodeGenerator::code_prototype_objects()
{
    int classtag = 1;

    for (auto& p : inherit_graph)
    {
        if (p.first->name == NOCLASS)
            continue;

        emit_label(p.first->name.get_val() + "_prototype");

        if (p.first->name == STRING)
            emit_word(STR_CLASS_TAG);
        else if (p.first->name == INTEGER)
            emit_word(INT_CLASS_TAG);
        else if (p.first->name == BOOLEAN)
            emit_word(BOOL_CLASS_TAG);
        else
            emit_word(classtag++);

        emit_word(OBJECT_HEADER_SIZE + calc_obj_size(p.first));
        emit_word(p.first->name.get_val() + "_disptable");
        emit_obj_attribs(p.first);
    }
}

void AstNodeCodeGenerator::emit_initial_data()
{
    os << ".data\n"
       << "\t.align\t2\n"
       // << "\t.globl\tclass_name_table\n"
       << "\t.globl\tMain_prototype\n"
       << "\t.globl\tMain_init\n"
       << "\t.globl\tMain.main\n"
       << "\t.globl\tbool_const0\n"
       << "\t.globl\tbool_const1\n"
       << "\t.globl\t__int_tag\n"
       << "\t.globl\t__bool_tag\n"
       << "\t.globl\t__string_tag\n"
       << "__int_tag:\n"
       << "\t.word\t" << INT_CLASS_TAG << "\n"
       << "__bool_tag:\n"
       << "\t.word\t" << BOOL_CLASS_TAG << "\n"
       << "__string_tag:\n"
       << "\t.word\t" << STR_CLASS_TAG << "\n";
}


void AstNodeCodeGenerator::visit(Program& prog)
{
    emit_initial_data();
    code_constants();
    //code_class_name_table();
    //code_prototype_table();

    for (auto& p : inherit_graph)
    {
        if (p.first->name != NOCLASS)
        {
            emit_label(p.first->name.get_val() + "_disptable");
            code_dispatch_table(p.first);
        }
    }

    code_prototype_objects();

    os << ".text\n";

    for (auto& cs : prog.classes)
        cs->accept(*this);
}

void AstNodeCodeGenerator::visit(Class& cs)
{
    // as each class node is traversed, its _init method (akin to constructor)
    // is also generated
    var_env.enter_scope();
    curr_class = cs.name;
    emit_label(cs.name.get_val() + "_init");
    emit_push(AR_BASE_SIZE);

    // standard registers that are saved to the stack
    emit_sw("fp", 12, "sp");
    emit_sw("s0", 8, "sp");
    emit_sw("ra", 4, "sp");
    emit_addiu("fp", "sp", 4);
    emit_move("s0", "a0");

    // if the class is anything other than object, call the
    // base class init method
    if (cs.name != OBJECT)
        emit_jal(cs.parent.get_val() + "_init");

    for (auto& attrib : cs.attributes)
        attrib->accept(*this);

    emit_move("a0", "s0");
    emit_lw("fp", 12, "sp");
    emit_lw("s0", 8, "sp");
    emit_lw("ra", 4, "sp");
    emit_pop(AR_BASE_SIZE);
    emit_jr("ra");

    curr_attr_count = 0;

    for (auto& method : cs.methods)
        method->accept(*this);

    var_env.exit_scope();
}

void AstNodeCodeGenerator::visit(Attribute& attr)
{
    attr.init->accept(*this);

    ++curr_attr_count;
    attr_tbl[curr_class][attr.name] = curr_attr_count;

    // PRIM_SLOT refers to an attribute of a primitive type (eg. Bool, String, Int)
    // the current attribute counter is incremented by 2 since the starting offset
    // for an attribute in the object layout is offset 3 (offstet 0-2 being the headers)
    // and then multiplied by 4 since there are 4 bytes in a word
    if (attr.type_decl != PRIM_SLOT)
        emit_sw("a0", WORD_SIZE * (curr_attr_count + 2), "s0");
}

void AstNodeCodeGenerator::visit(Formal&)
{

}

void AstNodeCodeGenerator::visit(Method& method)
{
    if (utility::is_basic_class(curr_class))
        return;

    var_env.enter_scope();
    emit_label(curr_class.get_val() + "." + method.name.get_val());

    emit_sw("ra", 4, "sp");

    int curr_offset = 1;

    for (auto& formal : method.params)
        var_env.add(formal->name, curr_offset++);

    method.body->accept(*this);

    // refer to stack frame layout in header file
    std::size_t ar_size = AR_BASE_SIZE + method.params.size();
    emit_lw("fp", ar_size * WORD_SIZE, "sp");
    emit_lw("s0", ar_size * WORD_SIZE - WORD_SIZE, "sp");
    emit_lw("ra", 4, "sp");
    emit_pop(AR_BASE_SIZE + method.params.size());
    emit_jr("ra");

    var_env.exit_scope();
}

void AstNodeCodeGenerator::visit(StringConst& str)
{
    emit_la("a0", (std::string("str_const") + std::to_string(stringtable().get_idx(str.token.get_val()))).c_str());
}

void AstNodeCodeGenerator::visit(IntConst& int_const)
{
    emit_la("a0", (std::string("int_const") + std::to_string(inttable().get_idx(int_const.token.get_val()))).c_str());
}

void AstNodeCodeGenerator::visit(BoolConst& bool_const)
{
    if (bool_const.value)
        emit_la("a0", "bool_const1");
    else
        emit_la("a0", "bool_const0");
}

void AstNodeCodeGenerator::visit(New& new_node)
{
    emit_la("a0", new_node.type.get_val() + "_prototype");
    emit_jal("Object.copy");
    emit_jal(new_node.type.get_val() + "_init");
}

void AstNodeCodeGenerator::visit(IsVoid& isvoid)
{
    isvoid.expr->accept(*this);
    emit_jal("isvoid");
}

void AstNodeCodeGenerator::visit(CaseBranch& branch)
{
    branch.expr->accept(*this);
}

void AstNodeCodeGenerator::visit(Assign& assign)
{
    assign.rhs->accept(*this);
    boost::optional<int> offset(var_env.lookup(assign.name));

    // result of evaluating rhs of assignment
    // is expected to be in register $a0
    // also note that offset is not checked for null
    // because the semantic analyzer should've caught
    // any variable misuse by this point
    emit_sw("a0", *offset, "fp");
}

void AstNodeCodeGenerator::visit(Block& block)
{
    for (auto& expr : block.body)
        expr->accept(*this);
}

void AstNodeCodeGenerator::visit(If& ifstmt)
{
    ++if_count;
    std::string ifcnt(std::to_string(if_count));

    ifstmt.predicate->accept(*this);

    emit_la("t1", "bool_const1");
    emit_beq("a0", "t1", "iftrue" + ifcnt);
    ifstmt.iffalse->accept(*this);
    emit_b("ifend" + ifcnt);

    emit_label("iftrue" + ifcnt);
    ifstmt.iftrue->accept(*this);

    emit_label("ifend" + ifcnt);
}

void AstNodeCodeGenerator::visit(While& whilestmt)
{
    ++while_count;

    emit_label("whileloop" + std::to_string(while_count));
    whilestmt.predicate->accept(*this);
    emit_la("t1", "bool_const1");
    emit_bne("a0", "t1", "whileend" + std::to_string(while_count));

    whilestmt.body->accept(*this);

    emit_b("whileloop" + std::to_string(while_count));
    emit_label("whileend" + std::to_string(while_count));
    emit_li("a0", 0);
}

void AstNodeCodeGenerator::visit(Complement& comp)
{
    comp.expr->accept(*this);
    emit_lw("t1", 12, "a0");
    emit_not("t1", "t1");
    emit_sw("t1", 12, "a0");
}

void AstNodeCodeGenerator::visit(LessThan& lt)
{
    lt.lhs->accept(*this);
    emit_move("a1", "a0");

    lt.rhs->accept(*this);
    emit_jal("less");
}

void AstNodeCodeGenerator::visit(LessThanEqualTo& lteq)
{
    lteq.lhs->accept(*this);
    emit_move("a1", "a0");

    lteq.rhs->accept(*this);
    emit_jal("less_eq");
}

void AstNodeCodeGenerator::visit(EqualTo& eq)
{
    eq.lhs->accept(*this);
    emit_move("a1", "a0");

    eq.rhs->accept(*this);
    emit_jal("eq");
}

void AstNodeCodeGenerator::visit(Plus& plus)
{
    plus.lhs->accept(*this);
    emit_sw("a0", 0, "sp");
    emit_push(1);

    plus.rhs->accept(*this);
    emit_jal("Object.copy");
    emit_lw("t1", 4, "sp");
    emit_lw("t1", 12, "t1");
    emit_lw("t2", 12, "v0");
    emit_add("t1", "t1", "t2");
    emit_sw("t1", 12, "a0");
    emit_pop(1);
}

void AstNodeCodeGenerator::visit(Sub& sub)
{
    sub.lhs->accept(*this);
    emit_sw("a0", 0, "sp");
    emit_push(1);

    sub.rhs->accept(*this);
    emit_jal("Object.copy");
    emit_lw("t1", 4, "sp");
    emit_lw("t1", 12, "t1");
    emit_lw("t2", 12, "v0");
    emit_sub("t1", "t1", "t2");
    emit_sw("t1", 12, "a0");
    emit_pop(1);
}

void AstNodeCodeGenerator::visit(Mul& mul)
{
    mul.lhs->accept(*this);
    emit_sw("a0", 0, "sp");
    emit_push(1);

    mul.rhs->accept(*this);
    emit_jal("Object.copy");
    emit_lw("t1", 4, "sp");
    emit_lw("t1", 12, "t1");
    emit_lw("t2", 12, "v0");
    emit_mul("t1", "t1", "t2");
    emit_sw("t1", 12, "a0");
    emit_pop(1);
}

void AstNodeCodeGenerator::visit(Div& div)
{
    div.lhs->accept(*this);
    emit_sw("a0", 0, "sp");
    emit_push(1);

    div.rhs->accept(*this);
    emit_jal("Object.copy");
    emit_lw("t1", 4, "sp");
    emit_lw("t1", 12, "t1");
    emit_lw("t2", 12, "v0");
    emit_div("t1", "t1", "t2");
    emit_sw("t1", 12, "a0");
    emit_pop(1);
}

void AstNodeCodeGenerator::visit(Not& nt)
{
    nt.expr->accept(*this);
    emit_jal("lnot");
}

void AstNodeCodeGenerator::visit(StaticDispatch& sdisp)
{
    sdisp.obj->accept(*this);
    for (auto& e : sdisp.actual)
       e->accept(*this);
}

void AstNodeCodeGenerator::visit(DynamicDispatch& ddisp)
{
    std::size_t ar_size = AR_BASE_SIZE + ddisp.actual.size();

    emit_push(ar_size);
    emit_sw("fp", ar_size * WORD_SIZE, "sp");
    emit_sw("s0", ar_size * WORD_SIZE - WORD_SIZE, "sp");

    std::size_t formal_offset = 8;
    for (auto& e : ddisp.actual)
    {
        e->accept(*this);
        emit_sw("a0", formal_offset, "sp");
        formal_offset += WORD_SIZE;
    }

    emit_addiu("fp", "sp", 4);

    ddisp.obj->accept(*this);
    emit_lw("t1", 8, "a0");
    emit_lw("t1", method_tbl[ddisp.obj->type][ddisp.method] * WORD_SIZE, "t1");
    emit_jalr("t1");
}

void AstNodeCodeGenerator::visit(Let& let)
{
    let.init->accept(*this);
    let.body->accept(*this);
}

void AstNodeCodeGenerator::visit(Case& caze)
{
    caze.expr->accept(*this);
    for (auto& br : caze.branches)
        br->accept(*this);
}

void AstNodeCodeGenerator::visit(Object& obj)
{
    if (obj.name == SELF)
    {
        emit_move("a0", "s0");
    }
    else
    {
        // If the object name is not in in the current local scope,
        // check if it's an attribute of the current class
        boost::optional<int> offset(var_env.lookup(obj.name));
        if (offset)
            emit_lw("a0", *offset, "fp");
        else
            emit_lw("a0", WORD_SIZE * (attr_tbl[curr_class][obj.name] + 2), "s0");
    }
}

void AstNodeCodeGenerator::visit(NoExpr&)
{

}
