// $ANTLR !Unknown version! MAlice.g 2012-11-01 15:12:18

import org.antlr.runtime.*;
import java.util.Stack;
import java.util.List;
import java.util.ArrayList;

public class MAliceParser extends Parser {
    public static final String[] tokenNames = new String[] {
        "<invalid>", "<EOR>", "<DOWN>", "<UP>", "ID", "CHAR", "INT", "'The'", "'looking-glass'", "'hatta ()'", "'opened'", "'closed'", "'and'", "'but'", "'then'", "','", "'.'", "'said'", "'Alice.'", "'was a'", "'number'", "'letter'", "'too'", "'became'", "'drank'", "'ate'", "'^'", "'|'", "'&'", "'+'", "'/'", "'*'", "'%'", "'~'"
    };
    public static final int T__29=29;
    public static final int T__28=28;
    public static final int T__27=27;
    public static final int T__26=26;
    public static final int T__25=25;
    public static final int T__24=24;
    public static final int T__23=23;
    public static final int T__22=22;
    public static final int T__21=21;
    public static final int T__20=20;
    public static final int CHAR=5;
    public static final int INT=6;
    public static final int ID=4;
    public static final int EOF=-1;
    public static final int T__9=9;
    public static final int T__8=8;
    public static final int T__7=7;
    public static final int T__30=30;
    public static final int T__19=19;
    public static final int T__31=31;
    public static final int T__32=32;
    public static final int T__33=33;
    public static final int T__16=16;
    public static final int T__15=15;
    public static final int T__18=18;
    public static final int T__17=17;
    public static final int T__12=12;
    public static final int T__11=11;
    public static final int T__14=14;
    public static final int T__13=13;
    public static final int T__10=10;

    // delegates
    // delegators


        public MAliceParser(TokenStream input) {
            this(input, new RecognizerSharedState());
        }
        public MAliceParser(TokenStream input, RecognizerSharedState state) {
            super(input, state);
             
        }
        

    public String[] getTokenNames() { return MAliceParser.tokenNames; }
    public String getGrammarFileName() { return "MAlice.g"; }



    // $ANTLR start "program"
    // MAlice.g:3:1: program : 'The' 'looking-glass' 'hatta ()' 'opened' statementList 'closed' EOF ;
    public final void program() throws RecognitionException {
        try {
            // MAlice.g:3:8: ( 'The' 'looking-glass' 'hatta ()' 'opened' statementList 'closed' EOF )
            // MAlice.g:3:10: 'The' 'looking-glass' 'hatta ()' 'opened' statementList 'closed' EOF
            {
            match(input,7,FOLLOW_7_in_program9); 
            match(input,8,FOLLOW_8_in_program11); 
            match(input,9,FOLLOW_9_in_program13); 
            match(input,10,FOLLOW_10_in_program15); 
            pushFollow(FOLLOW_statementList_in_program17);
            statementList();

            state._fsp--;

            match(input,11,FOLLOW_11_in_program19); 
            match(input,EOF,FOLLOW_EOF_in_program21); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "program"


    // $ANTLR start "statementList"
    // MAlice.g:5:1: statementList : ( statement ( 'and' | 'but' | 'then' | ',' | '.' ) statementList | printReturn );
    public final void statementList() throws RecognitionException {
        try {
            // MAlice.g:5:14: ( statement ( 'and' | 'but' | 'then' | ',' | '.' ) statementList | printReturn )
            int alt1=2;
            int LA1_0 = input.LA(1);

            if ( (LA1_0==ID) ) {
                int LA1_1 = input.LA(2);

                if ( (LA1_1==19||(LA1_1>=23 && LA1_1<=25)) ) {
                    alt1=1;
                }
                else if ( (LA1_1==17||(LA1_1>=26 && LA1_1<=32)) ) {
                    alt1=2;
                }
                else {
                    NoViableAltException nvae =
                        new NoViableAltException("", 1, 1, input);

                    throw nvae;
                }
            }
            else if ( (LA1_0==INT||LA1_0==33) ) {
                alt1=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 1, 0, input);

                throw nvae;
            }
            switch (alt1) {
                case 1 :
                    // MAlice.g:5:16: statement ( 'and' | 'but' | 'then' | ',' | '.' ) statementList
                    {
                    pushFollow(FOLLOW_statement_in_statementList28);
                    statement();

                    state._fsp--;

                    if ( (input.LA(1)>=12 && input.LA(1)<=16) ) {
                        input.consume();
                        state.errorRecovery=false;
                    }
                    else {
                        MismatchedSetException mse = new MismatchedSetException(null,input);
                        throw mse;
                    }

                    pushFollow(FOLLOW_statementList_in_statementList50);
                    statementList();

                    state._fsp--;


                    }
                    break;
                case 2 :
                    // MAlice.g:5:79: printReturn
                    {
                    pushFollow(FOLLOW_printReturn_in_statementList54);
                    printReturn();

                    state._fsp--;


                    }
                    break;

            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "statementList"


    // $ANTLR start "printReturn"
    // MAlice.g:7:1: printReturn : expression 'said' 'Alice.' ;
    public final void printReturn() throws RecognitionException {
        try {
            // MAlice.g:7:12: ( expression 'said' 'Alice.' )
            // MAlice.g:7:14: expression 'said' 'Alice.'
            {
            pushFollow(FOLLOW_expression_in_printReturn62);
            expression();

            state._fsp--;

            match(input,17,FOLLOW_17_in_printReturn64); 
            match(input,18,FOLLOW_18_in_printReturn66); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "printReturn"


    // $ANTLR start "statement"
    // MAlice.g:9:1: statement : ( declaration | assignment | step );
    public final void statement() throws RecognitionException {
        try {
            // MAlice.g:9:10: ( declaration | assignment | step )
            int alt2=3;
            int LA2_0 = input.LA(1);

            if ( (LA2_0==ID) ) {
                switch ( input.LA(2) ) {
                case 19:
                    {
                    alt2=1;
                    }
                    break;
                case 23:
                    {
                    alt2=2;
                    }
                    break;
                case 24:
                case 25:
                    {
                    alt2=3;
                    }
                    break;
                default:
                    NoViableAltException nvae =
                        new NoViableAltException("", 2, 1, input);

                    throw nvae;
                }

            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 2, 0, input);

                throw nvae;
            }
            switch (alt2) {
                case 1 :
                    // MAlice.g:9:12: declaration
                    {
                    pushFollow(FOLLOW_declaration_in_statement73);
                    declaration();

                    state._fsp--;


                    }
                    break;
                case 2 :
                    // MAlice.g:9:26: assignment
                    {
                    pushFollow(FOLLOW_assignment_in_statement77);
                    assignment();

                    state._fsp--;


                    }
                    break;
                case 3 :
                    // MAlice.g:9:39: step
                    {
                    pushFollow(FOLLOW_step_in_statement81);
                    step();

                    state._fsp--;


                    }
                    break;

            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "statement"


    // $ANTLR start "declaration"
    // MAlice.g:11:1: declaration : ( ID 'was a' ( 'number' | 'letter' ) ) ( ( 'too' )? ) ;
    public final void declaration() throws RecognitionException {
        try {
            // MAlice.g:11:12: ( ( ID 'was a' ( 'number' | 'letter' ) ) ( ( 'too' )? ) )
            // MAlice.g:11:14: ( ID 'was a' ( 'number' | 'letter' ) ) ( ( 'too' )? )
            {
            // MAlice.g:11:14: ( ID 'was a' ( 'number' | 'letter' ) )
            // MAlice.g:11:15: ID 'was a' ( 'number' | 'letter' )
            {
            match(input,ID,FOLLOW_ID_in_declaration89); 
            match(input,19,FOLLOW_19_in_declaration91); 
            if ( (input.LA(1)>=20 && input.LA(1)<=21) ) {
                input.consume();
                state.errorRecovery=false;
            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                throw mse;
            }


            }

            // MAlice.g:11:49: ( ( 'too' )? )
            // MAlice.g:11:50: ( 'too' )?
            {
            // MAlice.g:11:50: ( 'too' )?
            int alt3=2;
            int LA3_0 = input.LA(1);

            if ( (LA3_0==22) ) {
                alt3=1;
            }
            switch (alt3) {
                case 1 :
                    // MAlice.g:11:50: 'too'
                    {
                    match(input,22,FOLLOW_22_in_declaration103); 

                    }
                    break;

            }


            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "declaration"


    // $ANTLR start "assignment"
    // MAlice.g:13:1: assignment : ( ID 'became' expression | ID 'became' CHAR );
    public final void assignment() throws RecognitionException {
        try {
            // MAlice.g:13:11: ( ID 'became' expression | ID 'became' CHAR )
            int alt4=2;
            int LA4_0 = input.LA(1);

            if ( (LA4_0==ID) ) {
                int LA4_1 = input.LA(2);

                if ( (LA4_1==23) ) {
                    int LA4_2 = input.LA(3);

                    if ( (LA4_2==CHAR) ) {
                        alt4=2;
                    }
                    else if ( (LA4_2==ID||LA4_2==INT||LA4_2==33) ) {
                        alt4=1;
                    }
                    else {
                        NoViableAltException nvae =
                            new NoViableAltException("", 4, 2, input);

                        throw nvae;
                    }
                }
                else {
                    NoViableAltException nvae =
                        new NoViableAltException("", 4, 1, input);

                    throw nvae;
                }
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 4, 0, input);

                throw nvae;
            }
            switch (alt4) {
                case 1 :
                    // MAlice.g:13:13: ID 'became' expression
                    {
                    match(input,ID,FOLLOW_ID_in_assignment112); 
                    match(input,23,FOLLOW_23_in_assignment114); 
                    pushFollow(FOLLOW_expression_in_assignment116);
                    expression();

                    state._fsp--;


                    }
                    break;
                case 2 :
                    // MAlice.g:13:38: ID 'became' CHAR
                    {
                    match(input,ID,FOLLOW_ID_in_assignment120); 
                    match(input,23,FOLLOW_23_in_assignment122); 
                    match(input,CHAR,FOLLOW_CHAR_in_assignment124); 

                    }
                    break;

            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "assignment"


    // $ANTLR start "step"
    // MAlice.g:15:1: step : ID ( 'drank' | 'ate' ) ;
    public final void step() throws RecognitionException {
        try {
            // MAlice.g:15:5: ( ID ( 'drank' | 'ate' ) )
            // MAlice.g:15:8: ID ( 'drank' | 'ate' )
            {
            match(input,ID,FOLLOW_ID_in_step132); 
            if ( (input.LA(1)>=24 && input.LA(1)<=25) ) {
                input.consume();
                state.errorRecovery=false;
            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                throw mse;
            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "step"


    // $ANTLR start "expression"
    // MAlice.g:17:1: expression : or ( '^' or )* ;
    public final void expression() throws RecognitionException {
        try {
            // MAlice.g:17:11: ( or ( '^' or )* )
            // MAlice.g:17:13: or ( '^' or )*
            {
            pushFollow(FOLLOW_or_in_expression147);
            or();

            state._fsp--;

            // MAlice.g:17:16: ( '^' or )*
            loop5:
            do {
                int alt5=2;
                int LA5_0 = input.LA(1);

                if ( (LA5_0==26) ) {
                    alt5=1;
                }


                switch (alt5) {
            	case 1 :
            	    // MAlice.g:17:17: '^' or
            	    {
            	    match(input,26,FOLLOW_26_in_expression150); 
            	    pushFollow(FOLLOW_or_in_expression152);
            	    or();

            	    state._fsp--;


            	    }
            	    break;

            	default :
            	    break loop5;
                }
            } while (true);


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "expression"


    // $ANTLR start "or"
    // MAlice.g:19:1: or : and ( '|' and )* ;
    public final void or() throws RecognitionException {
        try {
            // MAlice.g:19:3: ( and ( '|' and )* )
            // MAlice.g:19:6: and ( '|' and )*
            {
            pushFollow(FOLLOW_and_in_or162);
            and();

            state._fsp--;

            // MAlice.g:19:10: ( '|' and )*
            loop6:
            do {
                int alt6=2;
                int LA6_0 = input.LA(1);

                if ( (LA6_0==27) ) {
                    alt6=1;
                }


                switch (alt6) {
            	case 1 :
            	    // MAlice.g:19:11: '|' and
            	    {
            	    match(input,27,FOLLOW_27_in_or165); 
            	    pushFollow(FOLLOW_and_in_or167);
            	    and();

            	    state._fsp--;


            	    }
            	    break;

            	default :
            	    break loop6;
                }
            } while (true);


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "or"


    // $ANTLR start "and"
    // MAlice.g:21:1: and : add ( '&' add )* ;
    public final void and() throws RecognitionException {
        try {
            // MAlice.g:21:4: ( add ( '&' add )* )
            // MAlice.g:21:7: add ( '&' add )*
            {
            pushFollow(FOLLOW_add_in_and178);
            add();

            state._fsp--;

            // MAlice.g:21:11: ( '&' add )*
            loop7:
            do {
                int alt7=2;
                int LA7_0 = input.LA(1);

                if ( (LA7_0==28) ) {
                    alt7=1;
                }


                switch (alt7) {
            	case 1 :
            	    // MAlice.g:21:12: '&' add
            	    {
            	    match(input,28,FOLLOW_28_in_and181); 
            	    pushFollow(FOLLOW_add_in_and183);
            	    add();

            	    state._fsp--;


            	    }
            	    break;

            	default :
            	    break loop7;
                }
            } while (true);


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "and"


    // $ANTLR start "add"
    // MAlice.g:23:1: add : ones ( '+' ones )* ;
    public final void add() throws RecognitionException {
        try {
            // MAlice.g:23:4: ( ones ( '+' ones )* )
            // MAlice.g:23:7: ones ( '+' ones )*
            {
            pushFollow(FOLLOW_ones_in_add193);
            ones();

            state._fsp--;

            // MAlice.g:23:12: ( '+' ones )*
            loop8:
            do {
                int alt8=2;
                int LA8_0 = input.LA(1);

                if ( (LA8_0==29) ) {
                    alt8=1;
                }


                switch (alt8) {
            	case 1 :
            	    // MAlice.g:23:13: '+' ones
            	    {
            	    match(input,29,FOLLOW_29_in_add196); 
            	    pushFollow(FOLLOW_ones_in_add198);
            	    ones();

            	    state._fsp--;


            	    }
            	    break;

            	default :
            	    break loop8;
                }
            } while (true);


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "add"


    // $ANTLR start "ones"
    // MAlice.g:25:1: ones : term ( ( '/' | '*' | '%' ) term )* ;
    public final void ones() throws RecognitionException {
        try {
            // MAlice.g:25:5: ( term ( ( '/' | '*' | '%' ) term )* )
            // MAlice.g:25:8: term ( ( '/' | '*' | '%' ) term )*
            {
            pushFollow(FOLLOW_term_in_ones208);
            term();

            state._fsp--;

            // MAlice.g:25:13: ( ( '/' | '*' | '%' ) term )*
            loop9:
            do {
                int alt9=2;
                int LA9_0 = input.LA(1);

                if ( ((LA9_0>=30 && LA9_0<=32)) ) {
                    alt9=1;
                }


                switch (alt9) {
            	case 1 :
            	    // MAlice.g:25:14: ( '/' | '*' | '%' ) term
            	    {
            	    if ( (input.LA(1)>=30 && input.LA(1)<=32) ) {
            	        input.consume();
            	        state.errorRecovery=false;
            	    }
            	    else {
            	        MismatchedSetException mse = new MismatchedSetException(null,input);
            	        throw mse;
            	    }

            	    pushFollow(FOLLOW_term_in_ones223);
            	    term();

            	    state._fsp--;


            	    }
            	    break;

            	default :
            	    break loop9;
                }
            } while (true);


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "ones"


    // $ANTLR start "term"
    // MAlice.g:27:1: term : ( ( '~' term ) | ID | INT );
    public final void term() throws RecognitionException {
        try {
            // MAlice.g:27:5: ( ( '~' term ) | ID | INT )
            int alt10=3;
            switch ( input.LA(1) ) {
            case 33:
                {
                alt10=1;
                }
                break;
            case ID:
                {
                alt10=2;
                }
                break;
            case INT:
                {
                alt10=3;
                }
                break;
            default:
                NoViableAltException nvae =
                    new NoViableAltException("", 10, 0, input);

                throw nvae;
            }

            switch (alt10) {
                case 1 :
                    // MAlice.g:27:8: ( '~' term )
                    {
                    // MAlice.g:27:8: ( '~' term )
                    // MAlice.g:27:9: '~' term
                    {
                    match(input,33,FOLLOW_33_in_term234); 
                    pushFollow(FOLLOW_term_in_term236);
                    term();

                    state._fsp--;


                    }


                    }
                    break;
                case 2 :
                    // MAlice.g:27:21: ID
                    {
                    match(input,ID,FOLLOW_ID_in_term241); 

                    }
                    break;
                case 3 :
                    // MAlice.g:27:26: INT
                    {
                    match(input,INT,FOLLOW_INT_in_term245); 

                    }
                    break;

            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "term"

    // Delegated rules


 

    public static final BitSet FOLLOW_7_in_program9 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_8_in_program11 = new BitSet(new long[]{0x0000000000000200L});
    public static final BitSet FOLLOW_9_in_program13 = new BitSet(new long[]{0x0000000000000400L});
    public static final BitSet FOLLOW_10_in_program15 = new BitSet(new long[]{0x0000000200000050L});
    public static final BitSet FOLLOW_statementList_in_program17 = new BitSet(new long[]{0x0000000000000800L});
    public static final BitSet FOLLOW_11_in_program19 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_program21 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_statement_in_statementList28 = new BitSet(new long[]{0x000000000001F000L});
    public static final BitSet FOLLOW_set_in_statementList30 = new BitSet(new long[]{0x0000000200000050L});
    public static final BitSet FOLLOW_statementList_in_statementList50 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_printReturn_in_statementList54 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_expression_in_printReturn62 = new BitSet(new long[]{0x0000000000020000L});
    public static final BitSet FOLLOW_17_in_printReturn64 = new BitSet(new long[]{0x0000000000040000L});
    public static final BitSet FOLLOW_18_in_printReturn66 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_declaration_in_statement73 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_assignment_in_statement77 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_step_in_statement81 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ID_in_declaration89 = new BitSet(new long[]{0x0000000000080000L});
    public static final BitSet FOLLOW_19_in_declaration91 = new BitSet(new long[]{0x0000000000300000L});
    public static final BitSet FOLLOW_set_in_declaration93 = new BitSet(new long[]{0x0000000000400002L});
    public static final BitSet FOLLOW_22_in_declaration103 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ID_in_assignment112 = new BitSet(new long[]{0x0000000000800000L});
    public static final BitSet FOLLOW_23_in_assignment114 = new BitSet(new long[]{0x0000000200000050L});
    public static final BitSet FOLLOW_expression_in_assignment116 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ID_in_assignment120 = new BitSet(new long[]{0x0000000000800000L});
    public static final BitSet FOLLOW_23_in_assignment122 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_CHAR_in_assignment124 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ID_in_step132 = new BitSet(new long[]{0x0000000003000000L});
    public static final BitSet FOLLOW_set_in_step134 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_or_in_expression147 = new BitSet(new long[]{0x0000000004000002L});
    public static final BitSet FOLLOW_26_in_expression150 = new BitSet(new long[]{0x0000000200000050L});
    public static final BitSet FOLLOW_or_in_expression152 = new BitSet(new long[]{0x0000000004000002L});
    public static final BitSet FOLLOW_and_in_or162 = new BitSet(new long[]{0x0000000008000002L});
    public static final BitSet FOLLOW_27_in_or165 = new BitSet(new long[]{0x0000000200000050L});
    public static final BitSet FOLLOW_and_in_or167 = new BitSet(new long[]{0x0000000008000002L});
    public static final BitSet FOLLOW_add_in_and178 = new BitSet(new long[]{0x0000000010000002L});
    public static final BitSet FOLLOW_28_in_and181 = new BitSet(new long[]{0x0000000200000050L});
    public static final BitSet FOLLOW_add_in_and183 = new BitSet(new long[]{0x0000000010000002L});
    public static final BitSet FOLLOW_ones_in_add193 = new BitSet(new long[]{0x0000000020000002L});
    public static final BitSet FOLLOW_29_in_add196 = new BitSet(new long[]{0x0000000200000050L});
    public static final BitSet FOLLOW_ones_in_add198 = new BitSet(new long[]{0x0000000020000002L});
    public static final BitSet FOLLOW_term_in_ones208 = new BitSet(new long[]{0x00000001C0000002L});
    public static final BitSet FOLLOW_set_in_ones211 = new BitSet(new long[]{0x0000000200000050L});
    public static final BitSet FOLLOW_term_in_ones223 = new BitSet(new long[]{0x00000001C0000002L});
    public static final BitSet FOLLOW_33_in_term234 = new BitSet(new long[]{0x0000000200000050L});
    public static final BitSet FOLLOW_term_in_term236 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ID_in_term241 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_INT_in_term245 = new BitSet(new long[]{0x0000000000000002L});

}