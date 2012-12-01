/** \file
 *  This C header file was generated by $ANTLR version 3.2 Sep 23, 2009 12:02:23
 *
 *     -  From the grammar source file : MAlice.g
 *     -                            On : 2012-12-01 17:58:23
 *     -                for the parser : MAliceParserParser *
 * Editing it, at least manually, is not wise. 
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The parser MAliceParser has the callable functions (rules) shown below,
 * which will invoke the code for the associated rule in the source grammar
 * assuming that the input stream is pointing to a token/text stream that could begin
 * this rule.
 * 
 * For instance if you call the first (topmost) rule in a parser grammar, you will
 * get the results of a full parse, but calling a rule half way through the grammar will
 * allow you to pass part of a full token stream to the parser, such as for syntax checking
 * in editors and so on.
 *
 * The parser entry points are called indirectly (by function pointer to function) via
 * a parser context typedef pMAliceParser, which is returned from a call to MAliceParserNew().
 *
 * The methods in pMAliceParser are  as follows:
 *
 *  - MAliceParser_program_return      pMAliceParser->program(pMAliceParser)
 *  - MAliceParser_declarationList_return      pMAliceParser->declarationList(pMAliceParser)
 *  - MAliceParser_declaration_return      pMAliceParser->declaration(pMAliceParser)
 *  - MAliceParser_varDeclaration_return      pMAliceParser->varDeclaration(pMAliceParser)
 *  - MAliceParser_varOptions_return      pMAliceParser->varOptions(pMAliceParser)
 *  - MAliceParser_funcDeclaration_return      pMAliceParser->funcDeclaration(pMAliceParser)
 *  - MAliceParser_procDeclaration_return      pMAliceParser->procDeclaration(pMAliceParser)
 *  - MAliceParser_headerParams_return      pMAliceParser->headerParams(pMAliceParser)
 *  - MAliceParser_headerParamsList_return      pMAliceParser->headerParamsList(pMAliceParser)
 *  - MAliceParser_headerParam_return      pMAliceParser->headerParam(pMAliceParser)
 *  - MAliceParser_callParams_return      pMAliceParser->callParams(pMAliceParser)
 *  - MAliceParser_callParamsList_return      pMAliceParser->callParamsList(pMAliceParser)
 *  - MAliceParser_body_return      pMAliceParser->body(pMAliceParser)
 *  - MAliceParser_statementList_return      pMAliceParser->statementList(pMAliceParser)
 *  - MAliceParser_idOptions_return      pMAliceParser->idOptions(pMAliceParser)
 *  - MAliceParser_print_return      pMAliceParser->print(pMAliceParser)
 *  - MAliceParser_statement_return      pMAliceParser->statement(pMAliceParser)
 *  - MAliceParser_conditionalStatement_return      pMAliceParser->conditionalStatement(pMAliceParser)
 *  - MAliceParser_elseif_return      pMAliceParser->elseif(pMAliceParser)
 *  - MAliceParser_type_return      pMAliceParser->type(pMAliceParser)
 *  - MAliceParser_refType_return      pMAliceParser->refType(pMAliceParser)
 *  - MAliceParser_expression_return      pMAliceParser->expression(pMAliceParser)
 *  - MAliceParser_prec11_return      pMAliceParser->prec11(pMAliceParser)
 *  - MAliceParser_prec10_return      pMAliceParser->prec10(pMAliceParser)
 *  - MAliceParser_prec9_return      pMAliceParser->prec9(pMAliceParser)
 *  - MAliceParser_prec8_return      pMAliceParser->prec8(pMAliceParser)
 *  - MAliceParser_prec7_return      pMAliceParser->prec7(pMAliceParser)
 *  - MAliceParser_prec6_return      pMAliceParser->prec6(pMAliceParser)
 *  - MAliceParser_prec5_return      pMAliceParser->prec5(pMAliceParser)
 *  - MAliceParser_prec4_return      pMAliceParser->prec4(pMAliceParser)
 *  - MAliceParser_prec3_return      pMAliceParser->prec3(pMAliceParser)
 *  - MAliceParser_prec2_return      pMAliceParser->prec2(pMAliceParser)
 *  - MAliceParser_atom_return      pMAliceParser->atom(pMAliceParser)
 *  - MAliceParser_delimiter_return      pMAliceParser->delimiter(pMAliceParser)
 *
 * The return type for any particular rule is of course determined by the source
 * grammar file.
 */
// [The "BSD licence"]
// Copyright (c) 2005-2009 Jim Idle, Temporal Wave LLC
// http://www.temporal-wave.com
// http://www.linkedin.com/in/jimidle
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. The name of the author may not be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef	_MAliceParser_H
#define _MAliceParser_H
/* =============================================================================
 * Standard antlr3 C runtime definitions
 */
#include    <antlr3.h>

/* End of standard antlr 3 runtime definitions
 * =============================================================================
 */
 
#ifdef __cplusplus
extern "C" {
#endif

// Forward declare the context typedef so that we can use it before it is
// properly defined. Delegators and delegates (from import statements) are
// interdependent and their context structures contain pointers to each other
// C only allows such things to be declared if you pre-declare the typedef.
//
typedef struct MAliceParser_Ctx_struct MAliceParser, * pMAliceParser;



#ifdef	ANTLR3_WINDOWS
// Disable: Unreferenced parameter,							- Rules with parameters that are not used
//          constant conditional,							- ANTLR realizes that a prediction is always true (synpred usually)
//          initialized but unused variable					- tree rewrite variables declared but not needed
//          Unreferenced local variable						- lexer rule declares but does not always use _type
//          potentially unitialized variable used			- retval always returned from a rule 
//			unreferenced local function has been removed	- susually getTokenNames or freeScope, they can go without warnigns
//
// These are only really displayed at warning level /W4 but that is the code ideal I am aiming at
// and the codegen must generate some of these warnings by necessity, apart from 4100, which is
// usually generated when a parser rule is given a parameter that it does not use. Mostly though
// this is a matter of orthogonality hence I disable that one.
//
#pragma warning( disable : 4100 )
#pragma warning( disable : 4101 )
#pragma warning( disable : 4127 )
#pragma warning( disable : 4189 )
#pragma warning( disable : 4505 )
#pragma warning( disable : 4701 )
#endif

/* ========================
 * BACKTRACKING IS ENABLED
 * ========================
 */
typedef struct MAliceParser_program_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_program_return;

typedef struct MAliceParser_declarationList_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_declarationList_return;

typedef struct MAliceParser_declaration_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_declaration_return;

typedef struct MAliceParser_varDeclaration_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_varDeclaration_return;

typedef struct MAliceParser_varOptions_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_varOptions_return;

typedef struct MAliceParser_funcDeclaration_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_funcDeclaration_return;

typedef struct MAliceParser_procDeclaration_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_procDeclaration_return;

typedef struct MAliceParser_headerParams_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_headerParams_return;

typedef struct MAliceParser_headerParamsList_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_headerParamsList_return;

typedef struct MAliceParser_headerParam_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_headerParam_return;

typedef struct MAliceParser_callParams_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_callParams_return;

typedef struct MAliceParser_callParamsList_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_callParamsList_return;

typedef struct MAliceParser_body_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_body_return;

typedef struct MAliceParser_statementList_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_statementList_return;

typedef struct MAliceParser_idOptions_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_idOptions_return;

typedef struct MAliceParser_print_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_print_return;

typedef struct MAliceParser_statement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_statement_return;

typedef struct MAliceParser_conditionalStatement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_conditionalStatement_return;

typedef struct MAliceParser_elseif_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_elseif_return;

typedef struct MAliceParser_type_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_type_return;

typedef struct MAliceParser_refType_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_refType_return;

typedef struct MAliceParser_expression_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_expression_return;

typedef struct MAliceParser_prec11_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_prec11_return;

typedef struct MAliceParser_prec10_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_prec10_return;

typedef struct MAliceParser_prec9_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_prec9_return;

typedef struct MAliceParser_prec8_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_prec8_return;

typedef struct MAliceParser_prec7_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_prec7_return;

typedef struct MAliceParser_prec6_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_prec6_return;

typedef struct MAliceParser_prec5_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_prec5_return;

typedef struct MAliceParser_prec4_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_prec4_return;

typedef struct MAliceParser_prec3_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_prec3_return;

typedef struct MAliceParser_prec2_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_prec2_return;

typedef struct MAliceParser_atom_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_atom_return;

typedef struct MAliceParser_delimiter_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_delimiter_return;



/** Context tracking structure for MAliceParser
 */
struct MAliceParser_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_PARSER   pParser;


     MAliceParser_program_return (*program)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_declarationList_return (*declarationList)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_declaration_return (*declaration)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_varDeclaration_return (*varDeclaration)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_varOptions_return (*varOptions)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_funcDeclaration_return (*funcDeclaration)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_procDeclaration_return (*procDeclaration)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_headerParams_return (*headerParams)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_headerParamsList_return (*headerParamsList)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_headerParam_return (*headerParam)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_callParams_return (*callParams)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_callParamsList_return (*callParamsList)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_body_return (*body)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_statementList_return (*statementList)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_idOptions_return (*idOptions)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_print_return (*print)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_statement_return (*statement)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_conditionalStatement_return (*conditionalStatement)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_elseif_return (*elseif)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_type_return (*type)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_refType_return (*refType)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_expression_return (*expression)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_prec11_return (*prec11)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_prec10_return (*prec10)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_prec9_return (*prec9)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_prec8_return (*prec8)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_prec7_return (*prec7)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_prec6_return (*prec6)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_prec5_return (*prec5)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_prec4_return (*prec4)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_prec3_return (*prec3)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_prec2_return (*prec2)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_atom_return (*atom)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_delimiter_return (*delimiter)	(struct MAliceParser_Ctx_struct * ctx);
     ANTLR3_BOOLEAN (*synpred1_MAlice)	(struct MAliceParser_Ctx_struct * ctx);
     ANTLR3_BOOLEAN (*synpred2_MAlice)	(struct MAliceParser_Ctx_struct * ctx);
     ANTLR3_BOOLEAN (*synpred3_MAlice)	(struct MAliceParser_Ctx_struct * ctx);
     ANTLR3_BOOLEAN (*synpred4_MAlice)	(struct MAliceParser_Ctx_struct * ctx);
     ANTLR3_BOOLEAN (*synpred5_MAlice)	(struct MAliceParser_Ctx_struct * ctx);
     ANTLR3_BOOLEAN (*synpred6_MAlice)	(struct MAliceParser_Ctx_struct * ctx);
     ANTLR3_BOOLEAN (*synpred7_MAlice)	(struct MAliceParser_Ctx_struct * ctx);
     ANTLR3_BOOLEAN (*synpred8_MAlice)	(struct MAliceParser_Ctx_struct * ctx);
     ANTLR3_BOOLEAN (*synpred9_MAlice)	(struct MAliceParser_Ctx_struct * ctx);
     ANTLR3_BOOLEAN (*synpred10_MAlice)	(struct MAliceParser_Ctx_struct * ctx);
     ANTLR3_BOOLEAN (*synpred11_MAlice)	(struct MAliceParser_Ctx_struct * ctx);
    // Delegated rules
    const char * (*getGrammarFileName)();
    void	    (*free)   (struct MAliceParser_Ctx_struct * ctx);
    /* @headerFile.members() */
    pANTLR3_BASE_TREE_ADAPTOR	adaptor;
    pANTLR3_VECTOR_FACTORY		vectors;
    /* End @headerFile.members() */
};

// Function protoypes for the constructor functions that external translation units
// such as delegators and delegates may wish to call.
//
ANTLR3_API pMAliceParser MAliceParserNew         (pANTLR3_COMMON_TOKEN_STREAM instream);
ANTLR3_API pMAliceParser MAliceParserNewSSD      (pANTLR3_COMMON_TOKEN_STREAM instream, pANTLR3_RECOGNIZER_SHARED_STATE state);

/** Symbolic definitions of all the tokens that the parser will work with.
 * \{
 *
 * Antlr will define EOF, but we can't use that as it it is too common in
 * in C header files and that would be confusing. There is no way to filter this out at the moment
 * so we just undef it here for now. That isn't the value we get back from C recognizers
 * anyway. We are looking for ANTLR3_TOKEN_EOF.
 */
#ifdef	EOF
#undef	EOF
#endif
#ifdef	Tokens
#undef	Tokens
#endif 
#define WHILE      22
#define CHAR      31
#define DQ      36
#define EOF      -1
#define T__93      93
#define T__94      94
#define T__91      91
#define INC      17
#define T__92      92
#define T__90      90
#define RETURN      20
#define CPL      15
#define VAR      30
#define BODY      6
#define APOSTROPHE      34
#define COMMENT      37
#define T__99      99
#define T__98      98
#define T__97      97
#define VARDEC      9
#define T__96      96
#define T__95      95
#define T__80      80
#define T__81      81
#define T__82      82
#define T__83      83
#define NS      32
#define VARSTAT      12
#define STDIN      21
#define INT      35
#define T__85      85
#define T__84      84
#define T__87      87
#define T__86      86
#define T__89      89
#define T__88      88
#define WS      38
#define T__71      71
#define T__72      72
#define T__70      70
#define FUNC      14
#define ELSESTS      25
#define T__76      76
#define T__75      75
#define T__74      74
#define T__73      73
#define T__79      79
#define T__78      78
#define T__77      77
#define T__68      68
#define T__69      69
#define T__66      66
#define DEC      18
#define T__67      67
#define T__64      64
#define T__65      65
#define T__62      62
#define T__63      63
#define PROCDEC      5
#define ID      33
#define T__61      61
#define T__60      60
#define IFSTS      24
#define IF      26
#define T__55      55
#define T__56      56
#define T__57      57
#define T__58      58
#define PROG      4
#define T__51      51
#define EXPR      29
#define T__52      52
#define T__53      53
#define T__54      54
#define T__59      59
#define T__50      50
#define CHOICE      23
#define T__42      42
#define T__43      43
#define T__40      40
#define T__41      41
#define T__46      46
#define T__47      47
#define NEWARR      11
#define T__44      44
#define T__45      45
#define T__48      48
#define T__49      49
#define NEWVAR      10
#define T__101      101
#define T__100      100
#define PRINT      19
#define ELSEIF      28
#define HPL      8
#define ASSIGN      16
#define CATCHALL      39
#define COND      27
#define FUNCDEC      7
#define ARRMEMBER      13
#ifdef	EOF
#undef	EOF
#define	EOF	ANTLR3_TOKEN_EOF
#endif

#ifndef TOKENSOURCE
#define TOKENSOURCE(lxr) lxr->pLexer->rec->state->tokSource
#endif

/* End of token definitions for MAliceParser
 * =============================================================================
 */
/** \} */

#ifdef __cplusplus
}
#endif

#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */
