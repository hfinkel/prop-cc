///////////////////////////////////////////////////////////////////////////////
//  This file is generated automatically using Prop (version 2.4.0),
//  last updated on Jul 1, 2011.
//  The original source file is "lawgen.pcc".
///////////////////////////////////////////////////////////////////////////////

#define PROP_TUPLE2_USED
#include <propdefs.h>
#line 1 "lawgen.pcc"
///////////////////////////////////////////////////////////////////////////////
//
//  Module to generate law abbreviations.
//
///////////////////////////////////////////////////////////////////////////////

#include "ir.h"
#include "ast.h"
#include "datagen.h"
#include "type.h"
#include "hashtab.h"
#include "list.h"

///////////////////////////////////////////////////////////////////////////////
//
//  Law environment.
//
///////////////////////////////////////////////////////////////////////////////

HashTable DatatypeCompiler::law_env( string_hash, string_equal);

///////////////////////////////////////////////////////////////////////////////
//
//  Method to lookup a pattern law.
//
///////////////////////////////////////////////////////////////////////////////

Pat DatatypeCompiler::lookup_pat( Id id)
{
  HashTable::Entry * e = law_env.lookup(id);
  return e ? (Pat)e->v : NOpat;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Method to enter a new pattern
//
///////////////////////////////////////////////////////////////////////////////

void DatatypeCompiler::add_law( LawDef law_def)
{
  
#line 42 "lawgen.pcc"
#line 56 "lawgen.pcc"
{
#line 45 "lawgen.pcc"
  
  if (lookup_pat(law_def->id) != NOpat)
  {
       error ("%Lduplicated definition of pattern constructor '%s'\n",law_def->id);
  }
  else
  {
    // type check pattern
    Pat p = law_def->guard != NOexp ? GUARDpat( p, law_def->guard) : law_def->pat;
    law_env.insert( law_def->id, POLYpat( law_def->id, length( law_def->args), law_def->args, p, law_def->guard, law_def->invert));
  }
  
#line 56 "lawgen.pcc"
}
#line 57 "lawgen.pcc"
#line 57 "lawgen.pcc"

}

///////////////////////////////////////////////////////////////////////////////
//
//  Function 'invertible' checks whether a pattern is invertible into
//  a pseudo constructor.
//
///////////////////////////////////////////////////////////////////////////////

#line 67 "lawgen.pcc"
#line 93 "lawgen.pcc"
Bool invertible (Pat x_1);
Bool invertible (a_List<Pat> *  x_1);
Bool invertible (a_List<LabPat> *  x_1);
Bool invertible (Pat x_1)
{
  if (x_1) {
    switch (x_1->tag__) {
      case a_Pat::tag_APPpat: {
        if (((Pat_APPpat *)x_1)->_1) {
          switch (((Pat_APPpat *)x_1)->_1->tag__) {
            case a_Pat::tag_CONSpat: {
#line 71 "lawgen.pcc"
             return invertible(((Pat_APPpat *)x_1)->_2); 
#line 71 "lawgen.pcc"
              } break;
            default: {
              L1:; 
#line 86 "lawgen.pcc"
             bug("invertible: %p", x_1); return false; 
#line 86 "lawgen.pcc"
              } break;
          }
        } else { goto L1; }
        } break;
      case a_Pat::tag_TYPEDpat: {
#line 72 "lawgen.pcc"
       return invertible(((Pat_TYPEDpat *)x_1)->_1); 
#line 72 "lawgen.pcc"
        } break;
      case a_Pat::tag_ASpat: {
#line 73 "lawgen.pcc"
       return invertible(((Pat_ASpat *)x_1)->_2); 
#line 73 "lawgen.pcc"
        } break;
      case a_Pat::tag_CONTEXTpat: {
#line 74 "lawgen.pcc"
       return invertible(((Pat_CONTEXTpat *)x_1)->_2); 
#line 74 "lawgen.pcc"
        } break;
      case a_Pat::tag_TUPLEpat: {
#line 75 "lawgen.pcc"
       return invertible(((Pat_TUPLEpat *)x_1)->TUPLEpat); 
#line 75 "lawgen.pcc"
        } break;
      case a_Pat::tag_EXTUPLEpat: {
#line 76 "lawgen.pcc"
       return invertible(((Pat_EXTUPLEpat *)x_1)->EXTUPLEpat); 
#line 76 "lawgen.pcc"
        } break;
      case a_Pat::tag_RECORDpat: {
#line 77 "lawgen.pcc"
       return ! ((Pat_RECORDpat *)x_1)->_2 && invertible(((Pat_RECORDpat *)x_1)->_1); 
#line 77 "lawgen.pcc"
        } break;
      case a_Pat::tag_LISTpat: {
#line 81 "lawgen.pcc"
       return invertible(((Pat_LISTpat *)x_1)->head) && invertible(((Pat_LISTpat *)x_1)->tail); 
#line 81 "lawgen.pcc"
        } break;
      case a_Pat::tag_VECTORpat: {
#line 83 "lawgen.pcc"
       return ! ((Pat_VECTORpat *)x_1)->head_flex && ! ((Pat_VECTORpat *)x_1)->tail_flex &&
        invertible(((Pat_VECTORpat *)x_1)->len) && invertible(((Pat_VECTORpat *)x_1)->array) && invertible(((Pat_VECTORpat *)x_1)->elements);
        
#line 85 "lawgen.pcc"
        } break;
      case a_Pat::tag_GUARDpat: {
#line 78 "lawgen.pcc"
       return invertible(((Pat_GUARDpat *)x_1)->_1); 
#line 78 "lawgen.pcc"
        } break;
      case a_Pat::tag_MARKEDpat: {
#line 79 "lawgen.pcc"
       return invertible(((Pat_MARKEDpat *)x_1)->_2); 
#line 79 "lawgen.pcc"
        } break;
      case a_Pat::tag_WILDpat:
      case a_Pat::tag_ARRAYpat:
      case a_Pat::tag_APPENDpat:
      case a_Pat::tag_LOGICALpat: {
#line 70 "lawgen.pcc"
       return false; 
#line 70 "lawgen.pcc"
        } break;
      case a_Pat::tag_INDpat:
      case a_Pat::tag_IDpat:
      case a_Pat::tag_CONSpat:
      case a_Pat::tag_LITERALpat: {
        L2:; 
#line 68 "lawgen.pcc"
       return true; 
#line 68 "lawgen.pcc"
        } break;
      default: { goto L1; } break;
    }
  } else { goto L2; }
}
Bool invertible (a_List<Pat> *  x_1)
{
  if (x_1) {
#line 89 "lawgen.pcc"
   return invertible(x_1->_1) && invertible(x_1->_2); 
#line 89 "lawgen.pcc"
  } else {
#line 88 "lawgen.pcc"
   return true; 
#line 88 "lawgen.pcc"
  }
}
Bool invertible (a_List<LabPat> *  x_1)
{
  if (x_1) {
#line 92 "lawgen.pcc"
   return invertible(x_1->_1.pat) && invertible(x_1->_2); 
#line 92 "lawgen.pcc"
  } else {
#line 91 "lawgen.pcc"
   return true; 
#line 91 "lawgen.pcc"
  }
}
#line 93 "lawgen.pcc"
#line 93 "lawgen.pcc"


///////////////////////////////////////////////////////////////////////////////
//
//  Function to convert a pattern into a variable creation expression.
//
///////////////////////////////////////////////////////////////////////////////

Exp mkvariable( Pat p)
{
  
#line 103 "lawgen.pcc"
#line 111 "lawgen.pcc"
{
  Ty _V1 = deref_all(p->ty);
  if (_V1) {
    switch (_V1->tag__) {
      case a_Ty::tag_TYCONty: {
        if (boxed(((Ty_TYCONty *)_V1)->_1)) {
          switch (((Ty_TYCONty *)_V1)->_1->tag__) {
            case a_TyCon::tag_DATATYPEtycon: {
              if (
#line 105 "lawgen.pcc"
              (((TyCon_DATATYPEtycon *)((Ty_TYCONty *)_V1)->_1)->qualifiers & QUALunifiable)
#line 105 "lawgen.pcc"
) {
                
#line 106 "lawgen.pcc"
              return APPexp( IDexp( ((TyCon_DATATYPEtycon *)((Ty_TYCONty *)_V1)->_1)->terms[((TyCon_DATATYPEtycon *)((Ty_TYCONty *)_V1)->_1)->unit]->name), TUPLEexp(
#line 106 "lawgen.pcc"
#line 106 "lawgen.pcc"
                nil_1_
#line 106 "lawgen.pcc"
#line 106 "lawgen.pcc"
                )); 
#line 106 "lawgen.pcc"
              } else {
                
                L3:; 
#line 108 "lawgen.pcc"
                
                error( "%L%p with type %T is not unifiable\n", p, p->ty);
                return NOexp;
                
#line 111 "lawgen.pcc"
              }
              } break;
            default: { goto L3; } break;
          }
        } else { goto L3; }
        } break;
      default: { goto L3; } break;
    }
  } else { goto L3; }
}
#line 112 "lawgen.pcc"
#line 112 "lawgen.pcc"

}

///////////////////////////////////////////////////////////////////////////////
//
//  Function 'pat2exp' converts an invertible pattern into a
//  constructor expression.  Also extracts the bound variable bindings
//  at the same time.
//
///////////////////////////////////////////////////////////////////////////////

static 
#line 123 "lawgen.pcc"
a_List<Tuple2<Id, Ty> > *  
#line 123 "lawgen.pcc"
 boundvars = 
#line 123 "lawgen.pcc"
nil_1_
#line 123 "lawgen.pcc"
#line 123 "lawgen.pcc"
;
static Exps           actual_args = 
#line 124 "lawgen.pcc"
#line 124 "lawgen.pcc"
nil_1_
#line 124 "lawgen.pcc"
#line 124 "lawgen.pcc"
;
static Bool           application_error = false;
Bool   write_mode = false;

#line 128 "lawgen.pcc"
#line 196 "lawgen.pcc"
Exp pat2exp (Pat x_1);
Exps pat2exp (a_List<Pat> *  x_1);
LabExps pat2exp (a_List<LabPat> *  x_1);
Exp pat2constructor (Pat x_1);
Exp pat2unifier (Pat x_1);
Exp pat2exp (Pat x_1)
{
  if (x_1) {
    switch (x_1->tag__) {
      case a_Pat::tag_WILDpat: {
        if (write_mode) {
          
#line 164 "lawgen.pcc"
         return mkvariable(x_1); 
#line 164 "lawgen.pcc"
        } else {
          
          L4:; 
#line 165 "lawgen.pcc"
         bug("pat2exp: %p", x_1); return NOexp; 
#line 165 "lawgen.pcc"
        }
        } break;
      case a_Pat::tag_INDpat: {
        if ((actual_args != nil_1_)) {
          
#line 133 "lawgen.pcc"
          
          int k = 0;
          for_each(Exp, e, actual_args)
          {  if (k == ((Pat_INDpat *)x_1)->_2) return e;
             k++;
          }
          	    error("%LMissing argument %i in law variable %s\n",((Pat_INDpat *)x_1)->_2,((Pat_INDpat *)x_1)->_1);
            application_error = true;
            return NOexp;
          
#line 142 "lawgen.pcc"
        } else {
          
#line 144 "lawgen.pcc"
          
          boundvars = 
#line 145 "lawgen.pcc"
#line 145 "lawgen.pcc"
          list_1_(mkTuple2(((Pat_INDpat *)x_1)->_1,((Pat_INDpat *)x_1)->_3),boundvars)
#line 145 "lawgen.pcc"
#line 145 "lawgen.pcc"
          ;
          return IDexp(((Pat_INDpat *)x_1)->_1);
          
#line 147 "lawgen.pcc"
        }
        } break;
      case a_Pat::tag_IDpat: {
#line 131 "lawgen.pcc"
       return write_mode ? mkvariable(x_1) : IDexp(((Pat_IDpat *)x_1)->_1); 
#line 131 "lawgen.pcc"
        } break;
      case a_Pat::tag_CONSpat: {
        if (((Pat_CONSpat *)x_1)->CONSpat) {
#line 157 "lawgen.pcc"
         return IDexp(((Pat_CONSpat *)x_1)->CONSpat->name); 
#line 157 "lawgen.pcc"
        } else { goto L4; }
        } break;
      case a_Pat::tag_APPpat: {
        if (((Pat_APPpat *)x_1)->_1) {
          switch (((Pat_APPpat *)x_1)->_1->tag__) {
            case a_Pat::tag_CONSpat: {
#line 158 "lawgen.pcc"
             return CONSexp(((Pat_CONSpat *)((Pat_APPpat *)x_1)->_1)->CONSpat,
#line 158 "lawgen.pcc"
#line 158 "lawgen.pcc"
              nil_1_
#line 158 "lawgen.pcc"
#line 158 "lawgen.pcc"
              ,pat2exp(((Pat_APPpat *)x_1)->_2)); 
#line 158 "lawgen.pcc"
              } break;
            default: { goto L4; } break;
          }
        } else { goto L4; }
        } break;
      case a_Pat::tag_TYPEDpat: {
#line 149 "lawgen.pcc"
       return pat2exp(((Pat_TYPEDpat *)x_1)->_1); 
#line 149 "lawgen.pcc"
        } break;
      case a_Pat::tag_ASpat: {
#line 150 "lawgen.pcc"
       return pat2exp(((Pat_ASpat *)x_1)->_2); 
#line 150 "lawgen.pcc"
        } break;
      case a_Pat::tag_LITERALpat: {
#line 148 "lawgen.pcc"
       return LITERALexp(((Pat_LITERALpat *)x_1)->LITERALpat); 
#line 148 "lawgen.pcc"
        } break;
      case a_Pat::tag_CONTEXTpat: {
#line 151 "lawgen.pcc"
       return pat2exp(((Pat_CONTEXTpat *)x_1)->_2); 
#line 151 "lawgen.pcc"
        } break;
      case a_Pat::tag_TUPLEpat: {
#line 152 "lawgen.pcc"
       return TUPLEexp(pat2exp(((Pat_TUPLEpat *)x_1)->TUPLEpat)); 
#line 152 "lawgen.pcc"
        } break;
      case a_Pat::tag_EXTUPLEpat: {
#line 153 "lawgen.pcc"
       return EXTUPLEexp(pat2exp(((Pat_EXTUPLEpat *)x_1)->EXTUPLEpat)); 
#line 153 "lawgen.pcc"
        } break;
      case a_Pat::tag_RECORDpat: {
#line 154 "lawgen.pcc"
       return RECORDexp(pat2exp(((Pat_RECORDpat *)x_1)->_1)); 
#line 154 "lawgen.pcc"
        } break;
      case a_Pat::tag_LISTpat: {
#line 160 "lawgen.pcc"
       return LISTexp(((Pat_LISTpat *)x_1)->cons,((Pat_LISTpat *)x_1)->nil,pat2exp(((Pat_LISTpat *)x_1)->head),pat2exp(((Pat_LISTpat *)x_1)->tail)); 
#line 160 "lawgen.pcc"
        } break;
      case a_Pat::tag_VECTORpat: {
#line 162 "lawgen.pcc"
       return VECTORexp(((Pat_VECTORpat *)x_1)->cons,pat2exp(((Pat_VECTORpat *)x_1)->elements)); 
#line 162 "lawgen.pcc"
        } break;
      case a_Pat::tag_GUARDpat: {
#line 155 "lawgen.pcc"
       return pat2exp(((Pat_GUARDpat *)x_1)->_1); 
#line 155 "lawgen.pcc"
        } break;
      case a_Pat::tag_MARKEDpat: {
#line 156 "lawgen.pcc"
       return pat2exp(((Pat_MARKEDpat *)x_1)->_2); 
#line 156 "lawgen.pcc"
        } break;
      default: { goto L4; } break;
    }
  } else {
#line 129 "lawgen.pcc"
   return NOexp; 
#line 129 "lawgen.pcc"
  }
}
Exps pat2exp (a_List<Pat> *  x_1)
{
  if (x_1) {
#line 168 "lawgen.pcc"
   return 
#line 168 "lawgen.pcc"
#line 168 "lawgen.pcc"
    list_1_(pat2exp(x_1->_1),pat2exp(x_1->_2))
#line 168 "lawgen.pcc"
#line 168 "lawgen.pcc"
    ; 
#line 168 "lawgen.pcc"
  } else {
#line 167 "lawgen.pcc"
   return 
#line 167 "lawgen.pcc"
#line 167 "lawgen.pcc"
    nil_1_
#line 167 "lawgen.pcc"
#line 167 "lawgen.pcc"
    ; 
#line 167 "lawgen.pcc"
  }
}
LabExps pat2exp (a_List<LabPat> *  x_1)
{
  if (x_1) {
#line 172 "lawgen.pcc"
    
    LabExp labexp;
    labexp.label = x_1->_1.label;
    labexp.exp   = pat2exp(x_1->_1.pat);
    return 
#line 176 "lawgen.pcc"
#line 176 "lawgen.pcc"
    list_1_(labexp,pat2exp(x_1->_2))
#line 176 "lawgen.pcc"
#line 176 "lawgen.pcc"
    ;
    
#line 177 "lawgen.pcc"
  } else {
#line 170 "lawgen.pcc"
   return 
#line 170 "lawgen.pcc"
#line 170 "lawgen.pcc"
    nil_1_
#line 170 "lawgen.pcc"
#line 170 "lawgen.pcc"
    ; 
#line 170 "lawgen.pcc"
  }
}
Exp pat2constructor (Pat x_1)
{
#line 180 "lawgen.pcc"
  
  Bool mode_save = write_mode;
  write_mode = false;
  Exp  e = pat2exp(x_1);
  write_mode = mode_save;
  return e;
  
#line 186 "lawgen.pcc"
}
Exp pat2unifier (Pat x_1)
{
#line 189 "lawgen.pcc"
  
  Bool mode_save = write_mode;
  write_mode = true;
  Exp  e = pat2exp(x_1);
  write_mode = mode_save;
  return e;
  
#line 195 "lawgen.pcc"
}
#line 196 "lawgen.pcc"
#line 196 "lawgen.pcc"


///////////////////////////////////////////////////////////////////////////////
//
//  Method to lookup a pattern law.
//
///////////////////////////////////////////////////////////////////////////////

Exp DatatypeCompiler::lookup_law( Id id, Exps args)
{
  Pat pat = lookup_pat(id);
  
#line 207 "lawgen.pcc"
#line 234 "lawgen.pcc"
{
  if (pat) {
    switch (pat->tag__) {
      case a_Pat::tag_POLYpat: {
        if (
#line 209 "lawgen.pcc"
        (((Pat_POLYpat *)pat)->_6 == true)
#line 209 "lawgen.pcc"
) {
          
#line 210 "lawgen.pcc"
          
          if (invertible(((Pat_POLYpat *)pat)->_4) && ((Pat_POLYpat *)pat)->_5 == NOexp)
          {
            actual_args = args;
            application_error = false;
            if (((Pat_POLYpat *)pat)->_2 != length(args))
            {
              error( "%Larity mismatch between law %p and arguments %f\n",
          	            ((Pat_POLYpat *)pat)->_4, TUPLEexp(args));
              return NOexp;
            }
            Exp exp = pat2exp(((Pat_POLYpat *)pat)->_4);
            actual_args = 
#line 222 "lawgen.pcc"
#line 222 "lawgen.pcc"
          nil_1_
#line 222 "lawgen.pcc"
#line 222 "lawgen.pcc"
          ;
          if (application_error)
            error( "%Lcannot apply law %p with arguments %f\n",
                  ((Pat_POLYpat *)pat)->_4, TUPLEexp(args));
          return exp;
          }
          else
          {
          error( "%Llaw %s(...) = %p is not invertible\n",((Pat_POLYpat *)pat)->_1,((Pat_POLYpat *)pat)->_4);
          }
          
#line 232 "lawgen.pcc"
        } else {
          
          L5:; }
        } break;
      default: { goto L5; } break;
    }
  } else { goto L5; }
}
#line 234 "lawgen.pcc"
#line 234 "lawgen.pcc"

  return NOexp;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Method to generate pattern law definitions
//
///////////////////////////////////////////////////////////////////////////////

void DatatypeCompiler::gen_law_defs (LawDefs law_defs)
{
  for_each (LawDef, l, law_defs)
  {
    
#line 248 "lawgen.pcc"
#line 269 "lawgen.pcc"
{
#line 251 "lawgen.pcc"
  
  l->set_loc();
  
  // infer the type of the pattern.
  l->ty = type_of(l->pat);
  
  // If the law is invertible, generate
  // code for the laws
  if (l->invert)
  {  if (invertible(l->pat) && l->guard == NOexp)
    {
      boundvars = 
#line 262 "lawgen.pcc"
#line 262 "lawgen.pcc"
  nil_1_
#line 262 "lawgen.pcc"
#line 262 "lawgen.pcc"
  ;
  gen_law_inverse(l->loc(),l->id,l->args,pat2constructor(l->pat),l->ty);
  boundvars = 
#line 264 "lawgen.pcc"
#line 264 "lawgen.pcc"
  nil_1_
#line 264 "lawgen.pcc"
#line 264 "lawgen.pcc"
  ;
  }
  else
    error( "%Llaw %s(...) = %p is not invertible\n",l->id,l->pat);
  }
  
#line 269 "lawgen.pcc"
}
#line 270 "lawgen.pcc"
#line 270 "lawgen.pcc"

  }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Check if type can be a C++ parameter
//
///////////////////////////////////////////////////////////////////////////////

Bool is_parameter_type (Ty ty)
{
  
#line 282 "lawgen.pcc"
#line 285 "lawgen.pcc"
{
  Ty _V2 = deref(ty);
  if (_V2) {
    switch (_V2->tag__) {
      case a_Ty::tag_TYCONty: {
        if (boxed(((Ty_TYCONty *)_V2)->_1)) {
          switch (((Ty_TYCONty *)_V2)->_1->tag__) {
            case a_TyCon::tag_RECORDtycon:
            case a_TyCon::tag_ARRAYtycon: {
              L6:; 
#line 284 "lawgen.pcc"
              return false;
#line 284 "lawgen.pcc"
              } break;
            default: {
              L7:; 
#line 285 "lawgen.pcc"
              return true;
#line 285 "lawgen.pcc"
              } break;
          }
        } else {
          switch ((int)((Ty_TYCONty *)_V2)->_1) {
            case ((int)v_TUPLEtycon): { goto L6; } break;
            default: { goto L7; } break;
          }
        }
        } break;
      default: { goto L7; } break;
    }
  } else { goto L7; }
}
#line 286 "lawgen.pcc"
#line 286 "lawgen.pcc"

}

///////////////////////////////////////////////////////////////////////////////
//
//  Method to generate an inverse for a law definition
//
///////////////////////////////////////////////////////////////////////////////

void DatatypeCompiler::gen_law_inverse
   (const Loc * location, Id id, Ids args, Exp exp, Ty ty)
{ // Generate the header name

  pr(
      "%^%/"
      "%^// Definition of law %s"
      "%^%/"
      "%#"
      "%^inline %t(",
      id, location->begin_line, location->file_name, ty, id
    );

  // Generate the parameters
  Bool comma = false;
  
#line 310 "lawgen.pcc"
#line 342 "lawgen.pcc"
{
  for (;;) {
    if (args) {
#line 313 "lawgen.pcc"
      
      if (comma) pr(", ");
      
#line 315 "lawgen.pcc"
a_List<Tuple2<Id, Ty> > *  
#line 315 "lawgen.pcc"
     bvs = boundvars;
      Bool found = false;
      Bool mode_save = write_mode;
      write_mode = false;
      
#line 334 "lawgen.pcc"
      {
        for (;;) {
          if (bvs) {
#line 322 "lawgen.pcc"
            
            if (bvs->_1._1 == args->_1)
            {
              if (! is_parameter_type(bvs->_1._2))
              { error( "%Llaw '%s': type %T cannot be used in parameter %s\n",
                      id, bvs->_1._2,bvs->_1._1);
              }
              found = true; pr("%t",bvs->_1._2,bvs->_1._1);
              bvs = 
#line 330 "lawgen.pcc"
#line 330 "lawgen.pcc"
            nil_1_
#line 330 "lawgen.pcc"
#line 330 "lawgen.pcc"
            ;
            }
            else
              bvs = bvs->_2;
            
#line 334 "lawgen.pcc"
          } else { goto L9; }
        }
        L9:;
      }
#line 335 "lawgen.pcc"
#line 335 "lawgen.pcc"
      
      if (! found)
        error( "%Llaw '%s': bound variable '%s' is absent in body %e\n",
               id, args->_1, exp);
      args  = args->_2;
      comma = true;
      write_mode = mode_save;
      
#line 342 "lawgen.pcc"
    } else { goto L8; }
  }
  L8:;
}
#line 343 "lawgen.pcc"
#line 343 "lawgen.pcc"


  // Generate the body
  pr(")%^{ return %e; }\n\n", exp);
}
#line 348 "lawgen.pcc"
/*
------------------------------- Statistics -------------------------------
Merge matching rules         = yes
Number of DFA nodes merged   = 141
Number of ifs generated      = 20
Number of switches generated = 10
Number of labels             = 7
Number of gotos              = 17
Adaptive matching            = enabled
Fast string matching         = disabled
Inline downcasts             = enabled
--------------------------------------------------------------------------
*/
