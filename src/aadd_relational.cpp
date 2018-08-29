/**
 
 @file aadd_relational.cpp
 
 @ingroup AADD
 
 @brief Relational operations with AADD.
 
 @author Carna Radojicic
 
 @copyright@parblock
 Copyright (c) 2017  Carna Radojicic Design of Cyber-Physical Systems
 TU Kaiserslautern Postfach 3049 67663 Kaiserslautern radojicic@cs.uni-kl.de
 
 This file is part of AADD package.
 
 AADD is free software: you can redistribute it and/or modify it
 under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 AADD is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public
 License for more details.
 
 You should have received a copy of the GNU General Public License
 along with AADD package. If not, see <http://www.gnu.org/licenses/>.
 @endparblock
 */

#include <math.h>

#include "aadd.h"
#include "aadd_lp_glpk.h"

/**
 @brief Private Method of AADD called by relational operators
 @details Compares AADD with threshold specified considering relational operator op
 @author Carna Radojicic
 @return the root of AADD
 @see relational operators 
 */
BDDNode* AADD::Compare(AADDNode* f,
                    double threshold,
                    vector<constraint<AAF> >  constraints,
                    string op) const
{
    BDDNode *res;    // result of comparison
    BDDNode *T, *E;  // Leaf nodes
    
    constraint<AAF>  cons;
    
    AAF cond;
    
    opt_sol bounds;
    
    BDDNode* one=ONE();
    BDDNode* zero=ZERO();
    
    if (f->isLeaf() )
    {
        
        AAF tmp=f->getValue();
        
        if (op=="==")
        {
            // check if tmp is equal to threshold
            
            if (tmp==threshold)
            {
                return one;
            }
            else
            {
                return zero;
            }
            
        }
        
        bounds=solve_lp(tmp, constraints);
        
        if (bounds.max<threshold and !(fabs(bounds.max-threshold)<1e-20))
        {
            if (op=="<=" or op=="<")
            {
                return one;
            }
            else
            {
                return zero;
            }
        }
        else if ((bounds.min>threshold) or fabs(bounds.min-threshold)<1e-20)
        {
            
            if (op=="<=" or op=="<")
            {
                return zero;
            }
            else
            {
                return one;
            }
            
            
        }
        else
        {
            AAF temp=threshold;
            cond=tmp-temp;
            
            unsigned long index=condMgr().addCond(cond);
            
            if (op=="<=" or op=="<")
            {
                res=new BDDNode(index,zero,one);
                return res;
            }
            else
            {
                res=new BDDNode(index,one,zero);
                return res;
            }
        }
    }
    
    /*Recursive step*/
  
    
    cons.con=f->getCond();
    
    cons.sign='+';
    
    constraints.push_back(cons);
    
    T=Compare(f->getT(),threshold, constraints, op);
    if (T == NULL) return(NULL);
    
    constraints.back().sign='-';
    
    E=Compare(f->getF(), threshold, constraints, op);
    
    if (E == NULL) {
        delete T;
        return(NULL);
    }
    if (T==E)
    {
        return T;
    }
    else
    {
        res=new BDDNode(f->getIndex(), T, E);
    }
    
    if (res == NULL) {
        delete T;
        delete E;
        return(NULL);
    }
    
    return res;
    
    
}


/**
 
  @brief Relational operator <=
 
  @details Compares AADD with AADD
 
  @author Carna Radojicic
 
  @return true if intervals or real numbers of leaf nodes of the left operand <br>
  are <= than intervals or real numbers of leaf nodes of the right operand <br>
  false if intervals or real numbers of leaf nodes of the left operand <br>
  are > than intervals or real numbers of leaf nodes of the right operand <br>
  UNKNOWN={false, true} otherwise  <br>
  All possible return values are represented by AADD
 
 */

BDD& AADD::operator<=(const AADD& right) const
{
    AADD temp((*this)-right);
    vector<constraint<AAF> > cons;
    double ths=0+1e-10;
    
    BDD* r=new BDD;
    r->setRoot(Compare(temp.getRoot(), ths, cons, "<="));
    cons.clear();
    return (*r);
} // AADD::operator <=

/**
 
 @brief Relational operator <
 
 @details Compares AADD with AADD
 
 @author Carna Radojicic
 
 @return true if intervals or real numbers of leaf nodes of the left operand <br>
 are < than intervals or real numbers of leaf nodes of the right operand <br>
 false if intervals or real numbers of leaf nodes of the left operand <br>
 are >= than intervals or real numbers of leaf nodes of the right operand <br>
 UNKNOWN={false, true} otherwise  <br>
 All possible return values are represented by AADD
 
 */

BDD& AADD::operator<( const AADD& right) const
{
    AADD diff((*this)-right);
    vector<constraint<AAF> > cons;
    BDD* r=new BDD;
    r->setRoot(Compare(diff.getRoot(), 0, cons, "<"));
    cons.clear();
    return (*r);
} // AADD::operator <

/**
 
 @brief Relational operator >=
 
 @details Compares AADD with AADD
 
 @author Carna Radojicic
 
 @return true if intervals or real numbers of leaf nodes of the left operand <br>
 are >= than intervals or real numbers of leaf nodes of the right operand <br>
 false if intervals or real numbers of leaf nodes of the left operand <br>
 are < than intervals or real numbers of leaf nodes of the right operand <br>
 UNKNOWN={false, true}  otherwise  <br>
 All possible return values are represented by AADD
 
 */

BDD& AADD::operator >= (const AADD& right) const
{
    AADD temp((*this)-right);
    vector<constraint<AAF> > cons;
    
    BDD* r=new BDD;
    r->setRoot(Compare(temp.getRoot(), 0, cons, ">="));
    cons.clear();
    return (*r);
} // AADD::operator >=

/**
 
 @brief Relational operator >
 
 @details Compares AADD with AADD
 @author Carna Radojicic, Christoph Grimm
 @return true if intervals or real numbers of leaf nodes of the left operand <br>
 are > than intervals or real numbers of leaf nodes of the right operand <br>
 false if intervals or real numbers of leaf nodes of the left operand <br>
 are <= than intervals or real numbers of leaf nodes of the right operand <br>
 UNKNOWN={false, true} otherwise  <br>
 All possible return values are represented by AADD
 
 */


BDD& AADD::operator>(const AADD& right) const
{
    AADD temp((*this)-right);
    vector<constraint<AAF> > cons;
    
    double ths=0+1e-10;
    BDD* r=new BDD;
    r->setRoot(Compare(temp.getRoot(),ths, cons, ">"));
    cons.clear();
    return (*r);
} // AADD::operator >


/**
 
 @brief Relational operator ==
 @details Compares AADD with AADD
 @author Carna Radojicic, Christoph Grimm
 @return true if intervals or real numbers of leaf nodes of the left operand <br>
 are equal to intervals or real numbers of leaf nodes of the right operand <br>
 false if they do not overlap, UNKNOWN={false, true} otherwise <br>
 All possible return values are represented by AADD
 
 */

BDD& AADD::operator==(const AADD& right) const
{

    AADD diff((*this)-right);
    vector<constraint<AAF> > cons;
    BDDNode *Temp, *Temp1;
    Temp=Compare(diff.getRoot(), 0, cons, ">=");
    Temp1=Compare(diff.getRoot(), 1e-10, cons, "<=");
    BDD *r=new BDD;
    r->setRoot(r->ApplyBinOp(And,Temp, Temp1));
    cons.clear();
    return (*r);
}

/**
 
 @brief Relational operator !=
 @details Compares two AADDs
 @author Carna Radojicic, Christoph Grimm
 @return negation of operator ==
 
 */


BDD& AADD::operator!=(const AADD& right) const
{
   
    return !((*this)==right);
    
    
}

/**
 
 @brief Relational operator ==
 
 @details Compares two BDDs
 
 @author Carna Radojicic, Christoph Grimm
 
 @return true if two BDDs are equal, otherwise false
 */

BDD& BDD::operator==(const BDD& right) const
{
    // if two BDDs are equal (a xor b) will be 0
    // we need to do negation to get correct result

    BDD *r=new BDD(((*this) xor right));
    
    return !(*r);
    
   
} // BDD::operator ==

/**
 
 @brief Relational operator ==
 
 @details Compares two BDDs
 
 @author Carna Radojicic, Christoph Grimm
 
 @return true if two BDDs are not equal, otherwise false
 */

BDD& BDD::operator!=(const BDD& right) const
{
     // if two BDDs are not equal (a xor b) will be 1
    BDD *r=new BDD((*this) xor right);
    return (*r);
    
} // BDD::operator !=


