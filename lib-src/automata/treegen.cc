///////////////////////////////////////////////////////////////////////////////
//  This file is generated automatically using Prop (version 2.4.0),
//  last updated on Jul 1, 2011.
//  The original source file is "treegen.pcc".
///////////////////////////////////////////////////////////////////////////////

#line 1 "treegen.pcc"
//////////////////////////////////////////////////////////////////////////////
// NOTICE:
//
// ADLib, Prop and their related set of tools and documentation are in the
// public domain.   The author(s) of this software reserve no copyrights on
// the source code and any code generated using the tools.  You are encouraged
// to use ADLib and Prop to develop software, in both academic and commercial
// settings, and are welcomed to incorporate any part of ADLib and Prop into
// your programs.
//
// Although you are under no obligation to do so, we strongly recommend that
// you give away all software developed using our tools.
//
// We also ask that credit be given to us when ADLib and/or Prop are used in
// your programs, and that this notice be preserved intact in all the source
// code.
//
// This software is still under development and we welcome(read crave for)
// any suggestions and help from the users.
//
// Allen Leung
// 1994
//////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include <AD/automata/treegram.h>  // tree grammar
#include <AD/automata/treegen.h>    // tree automata compiler
#include <AD/contain/bitset.h>      // bit set
#include <AD/hash/dchash.h>         // direct chaining hash map
#include <AD/contain/vararray.h>    // variable sized array
#include <AD/contain/slnklist.h>    // linked list
#include <AD/memory/mempool.h>      // memory pool

//////////////////////////////////////////////////////////////////////////////
//  Make hidden types visible
//////////////////////////////////////////////////////////////////////////////
typedef TreeGrammar::TreeProduction TreeProduction;
typedef TreeGrammar::Functor        Functor;
typedef TreeGrammar::Arity          Arity;
typedef TreeGrammar::Variable       Variable;
typedef TreeGrammar::State          State;
typedef TreeGrammar::Rule           Rule;
typedef TreeAutomaton::Equiv        Equiv;
typedef SLinkList<State>            StateList;

TreeTerm new_term(Mem& mem, short int f, unsigned char n, TreeTerm * subterms);
//////////////////////////////////////////////////////////////////////////////
//  Class TreeGenerator represents the internals of the automata compiler.
//  This stuff is completely hidden from the interface.
//////////////////////////////////////////////////////////////////////////////
class TreeGenerator {
  TreeGenerator(const TreeGenerator&);    // no copy constructor
  void operator = (const TreeGenerator&); // no assignment

  ///////////////////////////////////////////////////////////////////////////
  //  Internal data structures
  ///////////////////////////////////////////////////////////////////////////

public:
  TreeGen&                        treegen;
  TreeGrammar&                    G;             // the grammar
  MemPool                         mem;           // memory pool
  BitSet **                       ruleset_map;   // non-terminal -> rule set
  Rule *                          rule_map;      // non-terminal -> rule set
  DCHashTable<TreeTerm, Variable> non_terminals; // term -> non-terminals map
  VarArray <TreeTerm>             n_states;      // non-terminals -> term map
  DCHashTable<BitSet *, State>    state_labels;  // non-terminal set -> state mapping
  DCHashTable<TreeTerm, BitSet *> delta;         // term -> non-terminals mapping
  VarArray <BitSet *>             d_states;      // state -> non-terminal set
  VarArray <TreeTerm>             d_terms;       // state -> term map
  BitSet                      *** proj;          // projections
  BitSet                       ** closure0;      // transitive closure of each non-terminal
  Variable                        max_non_terminal;
  State                           number_of_states;
  StateList                   *** representers;

  ///////////////////////////////////////////////////////////////////////////
  //  Constructor
  ///////////////////////////////////////////////////////////////////////////
  TreeGenerator(TreeGen& gen, TreeGrammar& g) :
     treegen(gen), G(g), mem(4096), non_terminals(1037),
     state_labels(1037), delta(1037) {}

  ///////////////////////////////////////////////////////////////////////////
  //  Methods to process various phases of the compilation.
  ///////////////////////////////////////////////////////////////////////////
  void     assign_non_terminals   ();
  TreeTerm assign_non_terminal    ( TreeTerm );
  void     compute_closures       ();
  void     compute_projections    ();
  void     compute_representers   ();
  void     compute_initial_states ();
  void     compute_transitions    ();
  BitSet * compute_transition (BitSet *, TreeTerm, int, int, const BitSet&, State [], int []);
  void     compute_delta      (BitSet *, TreeTerm, int, int, const State []);
  void     compute_accept_rules (State);
  std::ostream& print_report (std::ostream&) const;
  std::ostream& print_state (std::ostream&, State) const;
};

//////////////////////////////////////////////////////////////////////////////
//  The following method is used to assign non-terminal symbols
//  to each distinct term within the grammar
//////////////////////////////////////////////////////////////////////////////
void TreeGenerator::assign_non_terminals()
{
  ///////////////////////////////////////////////////////////////////////////
  //  The grammar may already have user defined variable names.
  //  New non-terminals will have encodings right after these variables.
  ///////////////////////////////////////////////////////////////////////////
  max_non_terminal = G.max_variable() + 1;
  non_terminals.insert(wild_term,0);
  {
    for (Variable v = max_non_terminal - 1; v >= 0; v--)
      n_states[v] = wild_term;
  }

  ///////////////////////////////////////////////////////////////////////////
  //  Traverse thru the grammar and compute the non-terminal numbers.
  //  The tree grammar *IS* destructively altered.
  ///////////////////////////////////////////////////////////////////////////
  {
    for (int i = G.size() - 1; i >= 0; i--)
      G.update( i, assign_non_terminal( G[i].term));
  }

  ///////////////////////////////////////////////////////////////////////////
  //  Compute the mapping from non-terminal -> accept rule
  ///////////////////////////////////////////////////////////////////////////
  ruleset_map = (BitSet **)mem.c_alloc(sizeof(BitSet *) * max_non_terminal);
  rule_map    = (Rule *)   mem.c_alloc(sizeof(Rule) * max_non_terminal);
  {
    int i;
    for (i = max_non_terminal - 1; i >= 0; i--)
      rule_map[i] = -1;
    for (i = G.size() - 1; i >= 0; i--) {
      Variable v = non_terminals[G[i].term];
      if (ruleset_map[v] == 0)
        ruleset_map[v] = new (mem, max_non_terminal) BitSet;
      ruleset_map[v]->add(i);
      rule_map[v] = i;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//  Method to assign non-terminal numbers for a tree term.  This is
//  defined inductive as:
//////////////////////////////////////////////////////////////////////////////

TreeTerm TreeGenerator::assign_non_terminal( TreeTerm t)
{
  ///////////////////////////////////////////////////////////////////////////
  //  Step (a) recursively fold the terms.
  ///////////////////////////////////////////////////////////////////////////
  
#line 157 "treegen.pcc"
#line 167 "treegen.pcc"
{
  if (t) {
    switch (t->tag__) {
      case a_TreeTerm::tag_tree_term: {
#line 164 "treegen.pcc"
        
        for (int i = _tree_term(t)->_2 - 1; i >= 0; i--)
          _tree_term(t)->_3[i] = assign_non_terminal(_tree_term(t)->_3[i]);
        
#line 167 "treegen.pcc"
        } break;
      case a_TreeTerm::tag_var_term: {
#line 159 "treegen.pcc"
   // skip
        
#line 160 "treegen.pcc"
        } break;
      case a_TreeTerm::tag_and_term: {
#line 161 "treegen.pcc"
      _and_term(t)->_1 = assign_non_terminal(_and_term(t)->_1); _and_term(t)->_2 = assign_non_terminal(_and_term(t)->_2);
        
#line 162 "treegen.pcc"
        } break;
      case a_TreeTerm::tag_or_term: {
#line 162 "treegen.pcc"
     _or_term(t)->_1 = assign_non_terminal(_or_term(t)->_1); _or_term(t)->_2 = assign_non_terminal(_or_term(t)->_2);
        
#line 163 "treegen.pcc"
        } break;
      case a_TreeTerm::tag_not_term: {
#line 163 "treegen.pcc"
    _not_term(t)->not_term = assign_non_terminal(_not_term(t)->not_term);
        
#line 164 "treegen.pcc"
        } break;
      default: {
#line 160 "treegen.pcc"
   // skip
        
#line 161 "treegen.pcc"
        } break;
    }
  } else {
#line 158 "treegen.pcc"
  // skip
    
#line 159 "treegen.pcc"
  }
}
#line 167 "treegen.pcc"
#line 167 "treegen.pcc"


  ///////////////////////////////////////////////////////////////////////////
  //  Look it up from the ``non_terminal'' map.
  ///////////////////////////////////////////////////////////////////////////
  Ix p = non_terminals.lookup(t);
  if (p == 0)   // not found!
  {
    ////////////////////////////////////////////////////////////////////////
    //  Step (b) if it is not found, assign a new non-terminal number.
    //  Notice that ``wild_term'' always gets the non-terminal number of 0.
    ////////////////////////////////////////////////////////////////////////
    Variable var;
    
#line 180 "treegen.pcc"
#line 185 "treegen.pcc"
{
  if (t) {
    switch (t->tag__) {
      case a_TreeTerm::tag_var_term: {
#line 183 "treegen.pcc"
      var = _var_term(t)->var_term;
        
#line 184 "treegen.pcc"
        } break;
      default: {
#line 184 "treegen.pcc"
   var = max_non_terminal++;
        
#line 185 "treegen.pcc"
        } break;
    }
  } else {
#line 182 "treegen.pcc"
 var = 0;
    
#line 183 "treegen.pcc"
  }
}
#line 185 "treegen.pcc"
#line 185 "treegen.pcc"

    non_terminals.insert(t,var);  // update map
    n_states[var] = t;            // update inverse map
  }
  else
    t = non_terminals.key(p);

  return t;
}

//////////////////////////////////////////////////////////////////////////////
//  This is the method for computing transitive closure on a non-terminal
//  set.   Transitive closures apply in the presence of single reductions
//       X -> Y
//  or logical connectives:
//       X -> Y and Z
//       X -> Y or  Z
//
//  We don't support negation yet since it is anti-monotonic.
//////////////////////////////////////////////////////////////////////////////

void TreeGenerator::compute_closures()
{
  ///////////////////////////////////////////////////////////////////////////
  //  Allocate the array closure0, which maps each non-terminal to
  //  its transitive closure.  Closure set are allocated only if
  //  they are non-trivial (i.e. closure0[v] is something other than
  //  { 0, v }.
  ///////////////////////////////////////////////////////////////////////////
  closure0 = (BitSet**) mem.c_alloc(sizeof(BitSet*) * max_non_terminal);

  ///////////////////////////////////////////////////////////////////////////
  //  Allocate closures for user defined non-terminals
  ///////////////////////////////////////////////////////////////////////////
  {
    for (Variable v = G.min_variable(); v <= G.max_variable(); v++)
    {
      closure0[v] = new (mem, max_non_terminal) BitSet;
      closure0[v]->add(0);
      closure0[v]->add(v);
    }
  }

  ///////////////////////////////////////////////////////////////////////////
  //  Now compute the transitive closure
  ///////////////////////////////////////////////////////////////////////////
  Bool changed;
  BitSet& temp = *new(mem, max_non_terminal) BitSet; // temporary set buffer

  do
  {
    changed = false;

    /////////////////////////////////////////////////////////////////////////
    //  Propagate closure information.
    /////////////////////////////////////////////////////////////////////////
    foreach (p, non_terminals)
    {
      TreeTerm term = non_terminals.key(p);
      Variable var  = non_terminals.value(p);

      /////////////////////////////////////////////////////////////////////
      //  Allocate a new closure set if necessary.
      /////////////////////////////////////////////////////////////////////
      
#line 249 "treegen.pcc"
#line 257 "treegen.pcc"
{
  if (term) {
    switch (term->tag__) {
      case a_TreeTerm::tag_tree_term:
      case a_TreeTerm::tag_not_term:
      case a_TreeTerm::tag_set_term: {
        L1:; 
#line 256 "treegen.pcc"
        
        
#line 257 "treegen.pcc"
        } break;
      default: {
        if (
#line 251 "treegen.pcc"
        (closure0[var] == 0)
#line 251 "treegen.pcc"
) {
          
          L2:; 
#line 251 "treegen.pcc"
          
          closure0[var] = new (mem, max_non_terminal) BitSet;
          closure0[var]->add(0);
          closure0[var]->add(var);
          changed = true;
          
#line 256 "treegen.pcc"
        } else {
           goto L1; }
        } break;
    }
  } else { goto L1; }
}
#line 257 "treegen.pcc"
#line 257 "treegen.pcc"


      /////////////////////////////////////////////////////////////////////
      //  Propagate closures sets.
      /////////////////////////////////////////////////////////////////////
      
#line 262 "treegen.pcc"
#line 286 "treegen.pcc"
{
  if (term) {
    switch (term->tag__) {
      case a_TreeTerm::tag_var_term: {
        if (
#line 274 "treegen.pcc"
        (var != _var_term(term)->var_term)
#line 274 "treegen.pcc"
) {
          
#line 274 "treegen.pcc"
          
          if (closure0[_var_term(term)->var_term])
          {
            if (closure0[var]->Union_if_changed(*closure0[_var_term(term)->var_term]))
              changed = true;
          }
          else
          {
            if (closure0[var]->add_if_changed(_var_term(term)->var_term))
              changed = true;
          }
          
#line 285 "treegen.pcc"
        } else {
          
          L3:; 
#line 285 "treegen.pcc"
        // skip
          
#line 286 "treegen.pcc"
        }
        } break;
      case a_TreeTerm::tag_and_term: {
#line 264 "treegen.pcc"
        
        BitSet * s1 = closure0[ non_terminals[_and_term(term)->_1] ];
        BitSet * s2 = closure0[ non_terminals[_and_term(term)->_2] ];
        temp.Intersect(*s1,*s2);
        if (closure0[var]->Union_if_changed(temp)) changed = true;
        
#line 269 "treegen.pcc"
        } break;
      case a_TreeTerm::tag_or_term: {
#line 269 "treegen.pcc"
        
        BitSet * s1 = closure0[ non_terminals[_or_term(term)->_1] ];
        BitSet * s2 = closure0[ non_terminals[_or_term(term)->_2] ];
        if (closure0[var]->Union_if_changed(*s1)) changed = true;
        if (closure0[var]->Union_if_changed(*s2)) changed = true;
        
#line 274 "treegen.pcc"
        } break;
      default: { goto L3; } break;
    }
  } else { goto L3; }
}
#line 286 "treegen.pcc"
#line 286 "treegen.pcc"

    }

    /////////////////////////////////////////////////////////////////////////
    //  Propagate closure information to user non-terminals
    /////////////////////////////////////////////////////////////////////////
    for (int i = G.size() - 1; i >= 0; i--)
    {
      Variable v1 = G[i].var;
      Variable v2 = non_terminals[G[i].term];
      // cerr << "[" << v1 << " ";
      // G.print_variable(cerr,v1) << " <= " << v2 << " ";
      // G.print(cerr,G[i].term) << "]\n";
      if (v1 == 0) continue;
      if (closure0[v2])
      {
        if (closure0[v1]->Union_if_changed(*closure0[v2]))
          changed = true;
      }
      else
      {
        if (closure0[v1]->add_if_changed(v2))
          changed = true;
      }
    }
  } while (changed);
}

//////////////////////////////////////////////////////////////////////////////
//  This is the method for computing projections.
//  A projection on functor f and arity i (written as proj[f][i]) is
//  the set of non-terminals that can appear in that position.
//////////////////////////////////////////////////////////////////////////////

void TreeGenerator::compute_projections()
{
  ///////////////////////////////////////////////////////////////////////////
  //  Allocate the projection array.
  ///////////////////////////////////////////////////////////////////////////
  {
    proj = (BitSet***)mem.c_alloc(sizeof(BitSet**) * (G.max_functor() + 1));
    for (Functor f = G.min_functor(); f <= G.max_functor(); f++)
    {
      if (G.arity(f) == 0)
        continue;
      proj[f] = (BitSet**)mem.c_alloc(sizeof(BitSet*) * G.arity(f));
      for (int i = G.arity(f) - 1; i >= 0; i--)
      {
         proj[f][i] = new (mem, max_non_terminal) BitSet;
         proj[f][i]->add(0);
      }
    }
  }

  ///////////////////////////////////////////////////////////////////////////
  //  Compute the projections by going over the terms and
  //  looking up the non-terminals at each argument.
  ///////////////////////////////////////////////////////////////////////////
  {
    foreach (p, non_terminals) {
      
#line 346 "treegen.pcc"
#line 357 "treegen.pcc"
{
  TreeTerm _V1 = non_terminals.key(p);
  if (_V1) {
    switch (_V1->tag__) {
      case a_TreeTerm::tag_tree_term: {
#line 347 "treegen.pcc"
        
        for (int i = _tree_term(_V1)->_2 - 1; i >= 0; i--)
        {
          Variable v = non_terminals[ _tree_term(_V1)->_3[i] ];
          if (closure0[v])
            proj[_tree_term(_V1)->_1][i]->Union(*closure0[v]);
          else
            proj[_tree_term(_V1)->_1][i]->add(v);
        }
        
#line 356 "treegen.pcc"
        } break;
      default: {
        L4:; 
#line 356 "treegen.pcc"
      // skip
        
#line 357 "treegen.pcc"
        } break;
    }
  } else { goto L4; }
}
#line 357 "treegen.pcc"
#line 357 "treegen.pcc"

    }
  }
}

//////////////////////////////////////////////////////////////////////////////
// Method to allocate the representer state list for each functor at
// each arity.  ``Representers'' are simply interesting states.
//////////////////////////////////////////////////////////////////////////////

void TreeGenerator::compute_representers()
{
  representers = (StateList***) mem.c_alloc( sizeof(StateList**) * (G.max_functor() + 1));
  for (Functor f = G.min_functor(); f <= G.max_functor(); f++)
    representers[f] = (StateList**) mem.c_alloc( sizeof(StateList*) * G.arity(f));
}

//////////////////////////////////////////////////////////////////////////////
//  Method to compute the accept rules of a state
//////////////////////////////////////////////////////////////////////////////

void TreeGenerator::compute_accept_rules (State s)
{
  register const BitSet& vars     = *d_states[ s ];
  register BitSet&       rules    = treegen.accept_rules(s);
  register Rule          min_rule = G.size();
  register int v;
  foreach_bit_fast(v,vars)
  {
    Rule r = rule_map[v];
    if (r >= 0 && r < min_rule)
      min_rule = r;
    const BitSet * r_set = ruleset_map[v];
    if (r_set)
      rules.Union(*r_set);
  }
  if (min_rule == G.size())
    min_rule = -1;
  treegen.set_accept1(s, min_rule);
}

//////////////////////////////////////////////////////////////////////////////
//  Method to compute the initial states.
//  These are the ``wildcard'' or no-information state, state 0.
//  Followed by a state for each unit functor.
//////////////////////////////////////////////////////////////////////////////

void TreeGenerator::compute_initial_states()
{
  ///////////////////////////////////////////////////////////////////////////
  //  Create the first state corresponding to the wildcard.
  ///////////////////////////////////////////////////////////////////////////
  number_of_states = 0;
  {
    BitSet * state_zero = new (mem, max_non_terminal) BitSet;
    state_zero->add (0);
    state_labels.insert(state_zero, number_of_states);
    d_states[ number_of_states ] = state_zero;
    d_terms [ number_of_states ] = new(mem) 
#line 415 "treegen.pcc"
TreeTerm_set_term
#line 415 "treegen.pcc"
(state_zero);
    treegen.new_state(0);
    compute_accept_rules(0);
    number_of_states++;
  }

  ///////////////////////////////////////////////////////////////////////////
  //  Generate the rest of the terminal states formed by unit functors.
  ///////////////////////////////////////////////////////////////////////////
  {
    foreach (i, non_terminals) {
      
#line 426 "treegen.pcc"
#line 443 "treegen.pcc"
{
  TreeTerm _V2 = non_terminals.key(i);
  if (_V2) {
    switch (_V2->tag__) {
      case a_TreeTerm::tag_tree_term: {
        switch (_tree_term(_V2)->_2) {
          case 0: {
#line 427 "treegen.pcc"
          // use terms with arity 0 only
            BitSet * new_state = new (mem, max_non_terminal) BitSet;
            Variable v         = non_terminals.value(i);
            new_state->add(0);
            if (closure0[v])
              new_state->Union(*closure0[v]);
            else
              new_state->add(v);
            state_labels.insert(new_state, number_of_states);
            d_states[ number_of_states ] = new_state;
            d_terms [ number_of_states ] = 
#line 437 "treegen.pcc"
#line 437 "treegen.pcc"
            new (mem) TreeTerm_set_term(new_state)
#line 437 "treegen.pcc"
#line 437 "treegen.pcc"
            ;
            treegen.new_state(number_of_states);
            treegen.add_delta(_tree_term(_V2)->_1,number_of_states);
            compute_accept_rules(number_of_states);
            number_of_states++;
            
#line 442 "treegen.pcc"
          } break;
          default: {
            L5:; 
#line 442 "treegen.pcc"
            
            
#line 443 "treegen.pcc"
          }
        }
        } break;
      default: { goto L5; } break;
    }
  } else { goto L5; }
}
#line 443 "treegen.pcc"
#line 443 "treegen.pcc"

    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//  Method to compute transitions.
//////////////////////////////////////////////////////////////////////////////

void TreeGenerator::compute_transitions()
{
  ///////////////////////////////////////////////////////////////////////////
  //  Some temporary buffers used during this process.
  ///////////////////////////////////////////////////////////////////////////
  BitSet * projected   [256];            // projected states of arity i
  Bool     is_relevant [256];            // is arity i relevant?
  State    inputs      [256];            // input state of arity i
  int      equiv_class [256];            // equivalence class of above.
  BitSet * T    = 0;                     // current result non-terminal set
  TreeTerm term = new_term(mem, 0, 255); // current term

  {
    for (int i = G.max_arity() - 1; i >= 0; i--)
      projected[i] = new (mem, max_non_terminal) BitSet;
  }

  ///////////////////////////////////////////////////////////////////////////
  //  Compute the transitions of the states.  We'll start from state 0.
  ///////////////////////////////////////////////////////////////////////////

  for (State current = 0; current < number_of_states; current++)
  {
    const BitSet& S = *d_states[current];

    ////////////////////////////////////////////////////////////////////////
    // Iterate over all the non-unit functors and compute their transition
    // function on this new state ``current.''
    ////////////////////////////////////////////////////////////////////////
    for (Functor f = G.min_functor(); f <= G.max_functor(); f++)
    {
      int n; // arity
      if ((n = G.arity(f)) == 0)
        continue;

      
#line 487 "treegen.pcc"
#line 490 "treegen.pcc"
{
  if (term) {
    switch (term->tag__) {
      case a_TreeTerm::tag_tree_term: {
#line 489 "treegen.pcc"
      _tree_term(term)->_1 = f; _tree_term(term)->_2 = n; 
#line 489 "treegen.pcc"
        } break;
      default: {
        L6:; 
#line 490 "treegen.pcc"
      assert("bug"); 
#line 490 "treegen.pcc"
        } break;
    }
  } else { goto L6; }
}
#line 491 "treegen.pcc"
#line 491 "treegen.pcc"


      /////////////////////////////////////////////////////////////////////
      //  Compute the projections on the arguments.
      //  If the intersection of S with the ith projection is an old state T,
      //  then the transition with respect to arity ith must be the same
      //  as the state T.
      /////////////////////////////////////////////////////////////////////
      for (int i = n - 1; i >= 0; i--)
      {
        projected[i]->Intersect(S, *proj[f][i]);
        projected[i]->add(0);
        Ix old = state_labels.lookup(projected[i]);
        State mapped = current;
        if (old && (mapped = state_labels.value(old)) < current)
           is_relevant[i] = false;
        else
        {
          representers[f][i] = new (mem, mapped, representers[f][i]) StateList;
          is_relevant[i] = true;
        }
        //////////////////////////////////////////////////////////////////
        // Update the index map ``mu.''
        //////////////////////////////////////////////////////////////////
        treegen.add_mu(f,i,current,mapped);
      }

      /////////////////////////////////////////////////////////////////////
      // Now compute the transition function of functor f with respect
      // to the state ``current.''  Check only the arities that can
      // produce new information.  This is done by fixing all ``relevant''
      // arities to state ``current'' and the rest to the rest of the
      // representer states.
      /////////////////////////////////////////////////////////////////////
      for (int fix = n - 1; fix >= 0; fix--)
      {
        if (! is_relevant[fix])
          continue;
        inputs      [ fix ] = current;
        equiv_class [ fix ] = treegen.index_map(f,fix,current);
        T = compute_transition(T, term, 0, fix, *projected[fix], inputs, equiv_class);
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//  Method to compute a set of transitions functions.
//  We iterate over all the representer states.
//////////////////////////////////////////////////////////////////////////////

BitSet * TreeGenerator::compute_transition
            (
            BitSet *      T,        // temporary bitset
            TreeTerm      term,     // temporary term
            int           i,        // current arity to consider
            int           fixed,    // the fixed arity
            const BitSet& S,        // input state of the fixed arity
            State         inputs[], // input states
            int           equiv []  // equiv class of above
            )
{
  if (i == fixed) i++;  // skip the fixed arity.
  
#line 554 "treegen.pcc"
#line 619 "treegen.pcc"
{
  if (term) {
    switch (term->tag__) {
      case a_TreeTerm::tag_tree_term: {
#line 557 "treegen.pcc"
        
        if (i < _tree_term(term)->_2) {
          ////////////////////////////////////////////////////////////////////////
          //  Try arity i == wildcard first
          ////////////////////////////////////////////////////////////////////////
          _tree_term(term)->_3[i] = wild_term;
          inputs[i]   = 0;
          equiv [i]   = 0;
          T = compute_transition(T, term, i+1, fixed, S, inputs, equiv);
        
          //////////////////////////////////////////////////////////////////////
          // Try arity i == a representer state next.
          ////////////////////////////////////////////////////////////////////////
          for (StateList * r = representers[_tree_term(term)->_1][i]; r; r = r->tail)
          {
            inputs[i]   = r->head;
            equiv [i]   = treegen.index_map(_tree_term(term)->_1,i,r->head);
            _tree_term(term)->_3[i] = d_terms[ r->head ];
            T = compute_transition(T, term, i+1, fixed, S, inputs, equiv);
          }
        }
        else
        {
          ////////////////////////////////////////////////////////////////////////
          //  Compute the non-terminal set corresponding to the new state.
          ////////////////////////////////////////////////////////////////////////
          if (T == 0) T = new (mem, max_non_terminal) BitSet;
          else        T->clear();
          T->add(0);
          int v;
          foreach_bit_fast (v, S)
          {
            _tree_term(term)->_3[ fixed ] = n_states[v];
            compute_delta(T, term, 0, fixed, inputs);
          }
        
          ////////////////////////////////////////////////////////////////////////
          //  Now, lookup the new state to see if it is a new one.
          //  If so, create a new state.
          ////////////////////////////////////////////////////////////////////////
          Ix s;
          State mapped;
          if ((s = state_labels.lookup(T))) // old state
            mapped = state_labels.value(s);
          else // new state
          {
            d_states[ number_of_states ] = T;
            d_terms [ number_of_states ] = new(mem) 
#line 604 "treegen.pcc"
TreeTerm_set_term
#line 604 "treegen.pcc"
        (T);
        state_labels.insert(T, number_of_states);
        treegen.new_state(number_of_states);
        compute_accept_rules(number_of_states);
        T = 0;
        mapped = number_of_states++;
        }
        
        ////////////////////////////////////////////////////////////////////////
        //  Update the delta table.
        ////////////////////////////////////////////////////////////////////////
        treegen.add_delta(_tree_term(term)->_1, equiv, mapped);
        }
        
#line 617 "treegen.pcc"
        } break;
      default: {
        L7:; 
#line 619 "treegen.pcc"
       assert("bug"); 
#line 619 "treegen.pcc"
        } break;
    }
  } else { goto L7; }
}
#line 620 "treegen.pcc"
#line 620 "treegen.pcc"

  return T;
}

//////////////////////////////////////////////////////////////////////////////
//  Method to compute one delta function
//////////////////////////////////////////////////////////////////////////////

void TreeGenerator::compute_delta
     (
     BitSet *      T,        // temporary bitset
     TreeTerm      term,     // temporary term
     int           i,        // current arity to consider
     int           fixed,    // the fixed arity
     const State   inputs[]  // input states
     )
{
  Bool first = i == 0;
  if (i == fixed) i++;
  Ix p;

  if ((p = non_terminals.lookup(term)))
  {
    ////////////////////////////////////////////////////////////////////////
    // Found! use the info from the grammar.
    ////////////////////////////////////////////////////////////////////////
    Variable v = non_terminals.value(p);
    if (closure0[v]) T->Union(*closure0[v]);
    else T->add(v);
  }
  else if ((p = delta.lookup(term)))
  {
    ////////////////////////////////////////////////////////////////////////
    // Found! use the info from the delta map.
    ////////////////////////////////////////////////////////////////////////
    T->Union(* delta.value(p));
  }
  else
  {
    ////////////////////////////////////////////////////////////////////////
    // Split input state s into subcases and compute the union of all of
    // them.  Memorize individual cases into the map ``delta.''
    ////////////////////////////////////////////////////////////////////////
    
#line 663 "treegen.pcc"
#line 691 "treegen.pcc"
{
  if (term) {
    switch (term->tag__) {
      case a_TreeTerm::tag_tree_term: {
#line 666 "treegen.pcc"
        
        if (i < _tree_term(term)->_2)
        {
          const BitSet& U = *d_states[ inputs[i] ];
          TreeTerm save = _tree_term(term)->_3[i];
          BitSet * V;
          if (first) V = T;
          else { V = new (mem, max_non_terminal) BitSet; V->add(0); }
        
          int v;
          foreach_bit_fast (v, U)
          {
            _tree_term(term)->_3 [i] = n_states[v];
            compute_delta( V, term, i + 1, fixed, inputs);
          }
          _tree_term(term)->_3[i] = save;
        
          if (! first)
          {
            TreeTerm n_term = new_term( mem, _tree_term(term)->_1, _tree_term(term)->_2, _tree_term(term)->_3);
            delta.insert(n_term,V);
            T->Union(*V);
          }
        }
        
#line 690 "treegen.pcc"
        } break;
      default: {
        L8:; 
#line 691 "treegen.pcc"
       assert ("bug"); 
#line 691 "treegen.pcc"
        } break;
    }
  } else { goto L8; }
}
#line 692 "treegen.pcc"
#line 692 "treegen.pcc"

  }
}

//////////////////////////////////////////////////////////////////////////////
//  Method to print a report of the generated tables.
//////////////////////////////////////////////////////////////////////////////

std::ostream& TreeGenerator::print_report( std::ostream& log) const
{
  ///////////////////////////////////////////////////////////////////////////
  //  (1) Print the item set.
  ///////////////////////////////////////////////////////////////////////////
  {
    log << "\nItems:\n";
    for (Variable v = 0; v < max_non_terminal; v++)
    {
      log << '{' << v << "}\t";
      G.print(log, n_states[v]) << '\n';
    }
  }

  ///////////////////////////////////////////////////////////////////////////
  //  (2) Print each state
  ///////////////////////////////////////////////////////////////////////////
  {
    log << "\nStates:\n";
    for (State s = 0; s < number_of_states; s++)
      print_state(log,s);
  }
  return log;
}

//////////////////////////////////////////////////////////////////////////////
//  Method to print a state
//////////////////////////////////////////////////////////////////////////////

std::ostream& TreeGenerator::print_state( std::ostream& log, State s) const
{
  log << '<' << s << '>';
  const BitSet& S = *d_states[s];
  Variable v;
  foreach_bit(v,S) G.print(log << '\t', n_states[v]) << '\n';
  if (treegen.is_accept_state(s))
    log << "\t[accept " << treegen.accept1_rule(s) << "]\n";
  return log;
}

//////////////////////////////////////////////////////////////////////////////
//  Client visible methods follow.
//  All the work has been down by the class TreeGenerator.
//  Class TreeGen simply ties things up together.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//  Constructors and destructor of the class TreeGen.
//////////////////////////////////////////////////////////////////////////////

TreeGen:: TreeGen(Mem& m) : TreeAutomaton(m), impl(0) {}

TreeGen:: TreeGen(Mem& m, TreeGrammar& Gram)
   : TreeAutomaton(m), impl(0) { compile(Gram); }

TreeGen::~TreeGen() { delete impl; }

//////////////////////////////////////////////////////////////////////////////
//  This is the top level method to compile a tree grammar.
//////////////////////////////////////////////////////////////////////////////

void TreeGen::compile( TreeGrammar& Gram)
{
  ///////////////////////////////////////////////////////////////////////////
  //  Let our superclass do its stuff first.
  ///////////////////////////////////////////////////////////////////////////
  Super::compile(Gram);

  ///////////////////////////////////////////////////////////////////////////
  //  The internal representation is defined here.
  ///////////////////////////////////////////////////////////////////////////
  delete impl;
  impl = new TreeGenerator(*this,Gram);

  ///////////////////////////////////////////////////////////////////////////
  //  Now carry out the various steps.
  ///////////////////////////////////////////////////////////////////////////
  impl->assign_non_terminals();   // assign unique non-terminal names.
  impl->compute_closures();       // compute transitive closures of non-terminals.
  impl->compute_projections();    // compute the projections on functors.
  impl->compute_representers();   // allocate the representor states.
  impl->compute_initial_states(); // compute the initial states first.
  impl->compute_transitions();    // compute the rest of the transitions.
}

//////////////////////////////////////////////////////////////////////////////
//  Method to print a report of the generated tables.
//////////////////////////////////////////////////////////////////////////////

std::ostream& TreeGen::print_report( std::ostream& log) const
{
  Super::print_report(log);
  if (impl)
    impl->print_report(log);
  return log;
}

//////////////////////////////////////////////////////////////////////////////
//  Method to print a state of the generated tables.
//////////////////////////////////////////////////////////////////////////////

std::ostream& TreeGen::print_state( std::ostream& log, State s) const
{
  if (impl)
    impl->print_state( log, s);
  return log;
}

//////////////////////////////////////////////////////////////////////////////
//  Algorithm name
//////////////////////////////////////////////////////////////////////////////

const char * TreeGen::algorithm() const { return "TreeGen"; }
#line 813 "treegen.pcc"
/*
------------------------------- Statistics -------------------------------
Merge matching rules         = yes
Number of DFA nodes merged   = 52
Number of ifs generated      = 11
Number of switches generated = 10
Number of labels             = 8
Number of gotos              = 10
Adaptive matching            = disabled
Fast string matching         = disabled
Inline downcasts             = disabled
--------------------------------------------------------------------------
*/
