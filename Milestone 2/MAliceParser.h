/** \file
 *  This C header file was generated by $ANTLR version 3.2 Sep 23, 2009 12:02:23
 *
 *     -  From the grammar source file : MAlice.g
 *     -                            On : 2012-11-09 13:07:35
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
 *  - MAliceParser_subRule2_return      pMAliceParser->subRule2(pMAliceParser)
 *  - MAliceParser_subRule_return      pMAliceParser->subRule(pMAliceParser)
 *  - MAliceParser_statement_return      pMAliceParser->statement(pMAliceParser)
 *  - MAliceParser_conditionalStatement_return      pMAliceParser->conditionalStatement(pMAliceParser)
 *  - MAliceParser_type_return      pMAliceParser->type(pMAliceParser)
 *  - MAliceParser_refType_return      pMAliceParser->refType(pMAliceParser)
 *  - MAliceParser_expression_return      pMAliceParser->expression(pMAliceParser)
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

typedef struct MAliceParser_subRule2_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_subRule2_return;

typedef struct MAliceParser_subRule_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    MAliceParser_subRule_return;

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
     MAliceParser_subRule2_return (*subRule2)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_subRule_return (*subRule)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_statement_return (*statement)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_conditionalStatement_return (*conditionalStatement)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_type_return (*type)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_refType_return (*refType)	(struct MAliceParser_Ctx_struct * ctx);
     MAliceParser_expression_return (*expression)	(struct MAliceParser_Ctx_struct * ctx);
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
#define T__68      68
#define T__69      69
#define T__66      66
#define T__67      67
#define T__64      64
#define T__29      29
#define T__65      65
#define T__28      28
#define T__62      62
#define T__27      27
#define T__63      63
#define T__26      26
#define T__25      25
#define T__24      24
#define T__23      23
#define T__22      22
#define T__21      21
#define T__20      20
#define PROCDEC      5
#define CHAR      13
#define ID      11
#define T__61      61
#define T__60      60
#define EOF      -1
#define T__55      55
#define T__19      19
#define T__56      56
#define T__57      57
#define PROG      4
#define T__58      58
#define T__51      51
#define T__52      52
#define T__18      18
#define T__53      53
#define T__17      17
#define T__54      54
#define T__59      59
#define CPL      8
#define BODY      10
#define COMMENT      16
#define T__50      50
#define VARDEC      9
#define T__42      42
#define T__43      43
#define T__40      40
#define T__41      41
#define T__46      46
#define T__47      47
#define T__44      44
#define T__45      45
#define T__48      48
#define T__49      49
#define INT      14
#define HPL      7
#define T__30      30
#define T__31      31
#define T__32      32
#define WS      15
#define T__71      71
#define T__33      33
#define T__72      72
#define T__34      34
#define T__35      35
#define T__70      70
#define T__36      36
#define T__37      37
#define T__38      38
#define T__39      39
#define T__76      76
#define T__75      75
#define FUNCDEC      6
#define T__74      74
#define T__73      73
#define T__79      79
#define STRING      12
#define T__78      78
#define T__77      77
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
