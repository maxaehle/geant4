//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/*
 * =============================================================================
 *
 *       Filename:  CexmcAST.cc
 *
 *    Description:  abstract syntax tree for custom filter scripting language
 *
 *        Version:  1.0
 *        Created:  17.07.2010 14:45:14
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifdef CEXMC_USE_CUSTOM_FILTER

#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <boost/variant/get.hpp>
#include <boost/format.hpp>
#include "CexmcAST.hh"
#include "CexmcException.hh"


namespace  CexmcAST
{
    void  Subtree::Print( int  level ) const
    {
        static const std::string  opId[] =
            { "UNINITIALIZED", "TOP", "u -", "!", "*", "/", "+", "-", "<", "<=",
              ">", ">=", "=", "!=", "&", "|" };

        std::stringstream         value;
        const Operator *          op( boost::get< Operator >( &type ) );

        if ( op )
        {
            value << "-op- " << opId[ op->type ];
        }
        else
        {
            const Function *  fun( boost::get< Function >( &type ) );
            value << "-fun- " << *fun;
        }

        std::stringstream  format;
        format << "%|" << level * printIndent << "t|";
        std::cout << boost::format( format.str() ) << value.str() << std::endl;

        for ( std::vector< Node >::const_iterator  k( children.begin() );
                                                      k != children.end(); ++k )
        {
            const Subtree *  subtree( boost::get< Subtree >( &*k ) );

            if ( subtree )
            {
                subtree->Print( level + 1 );
            }
            else
            {
                const Leaf *  leaf( boost::get< Leaf >( &*k ) );
                if ( leaf )
                    PrintLeaf( leaf, level + 1 );
            }
        }
    }


    void  Subtree::PrintLeaf( const Leaf *  leaf, int  level ) const
    {
        const Variable *   variable( NULL );
        std::stringstream  value;

        if ( ( variable = boost::get< Variable >( leaf ) ) )
        {
            value << variable->name;
            if ( variable->index1 > 0 )
            {
                value << "[" << variable->index1;
                if ( variable->index2 > 0 )
                    value << "," << variable->index2;
                value << "]";
            }
        }
        else
        {
            const Constant *  constant( boost::get< Constant >( leaf ) );
            const int *       intConstant( boost::get< int >( constant ) );
            const G4double *    G4doubleConstant( boost::get< G4double >(
                                                                constant ) );

            value << ( intConstant ? *intConstant : *G4doubleConstant );
        }

        std::stringstream  format;
        format << "%|" << level * printIndent << "t|";
        std::cout << boost::format( format.str() ) << value.str() << std::endl;
    }


    BasicEval::~BasicEval()
    {
    }


    bool  BasicEval::operator()( const Subtree &  ast ) const
    {
        ScalarValueType  retval( GetScalarValue( ast ) );
        int *            intRetval( NULL );
        G4double *         G4doubleRetval( NULL );

        intRetval = boost::get< int >( &retval );

        if ( ! intRetval )
            G4doubleRetval = boost::get< G4double >( &retval );

        return G4doubleRetval ? bool( *G4doubleRetval ) : bool( *intRetval );
    }


    BasicEval::ScalarValueType  BasicEval::GetScalarValue(
                                                    const Node &  node ) const
    {
        const Subtree *  ast( boost::get< Subtree >( &node ) );

        if ( ast )
        {
            const Operator *  op( boost::get< Operator >( &ast->type ) );
            if ( op )
            {
                ScalarValueType           left( 0 );
                ScalarValueType           right( 0 );
                int *                     intLeft( NULL );
                G4double *                  G4doubleLeft( NULL );
                int *                     intRight( NULL );
                G4double *                  G4doubleRight( NULL );
                bool                      isDoubleRetval( false );

                if ( ast->children.size() > 0 )
                {
                    left = GetScalarValue( ast->children[ 0 ] );
                    intLeft = boost::get< int >( &left );
                    if ( ! intLeft )
                    {
                        G4doubleLeft = boost::get< G4double >( &left );
                        if ( ! G4doubleLeft )
                            throw CexmcException( CexmcCFUnexpectedContext );
                    }
                }

                switch ( op->type )
                {
                case And :
                case Or :
                    break;
                default :
                    if ( ast->children.size() > 1 )
                    {
                        right = GetScalarValue( ast->children[ 1 ] );
                        intRight = boost::get< int >( &right );
                        if ( ! intRight )
                        {
                            G4doubleRight = boost::get< G4double >( &right );
                            if ( ! G4doubleRight )
                                throw CexmcException(
                                                CexmcCFUnexpectedContext );
                        }
                    }
                    isDoubleRetval = G4doubleLeft || G4doubleRight;
                    break;
                }

                switch ( op->type )
                {
                case Uninitialized :
                    return 1;
                case Top :
                    return left;
                case UMinus :
                    if ( G4doubleLeft )
                        return - *G4doubleLeft;
                    else
                        return - *intLeft;
                case Not :
                    if ( G4doubleLeft )
                        return ! *G4doubleLeft;
                    else
                        return ! *intLeft;
                case Mult :
                    if ( isDoubleRetval )
                        return ( G4doubleLeft ? *G4doubleLeft : *intLeft ) *
                               ( G4doubleRight ? *G4doubleRight : *intRight );
                    else
                        return *intLeft * *intRight;
                case Div :
                    if ( isDoubleRetval )
                        return ( G4doubleLeft ? *G4doubleLeft : *intLeft ) /
                               ( G4doubleRight ? *G4doubleRight : *intRight );
                    else
                        return *intLeft / *intRight;
                case Plus :
                    if ( isDoubleRetval )
                        return ( G4doubleLeft ? *G4doubleLeft : *intLeft ) +
                               ( G4doubleRight ? *G4doubleRight : *intRight );
                    else
                        return *intLeft + *intRight;
                case Minus :
                    if ( isDoubleRetval )
                        return ( G4doubleLeft ? *G4doubleLeft : *intLeft ) -
                               ( G4doubleRight ? *G4doubleRight : *intRight );
                    else
                        return *intLeft - *intRight;
                case Less :
                    if ( isDoubleRetval )
                        return ( G4doubleLeft ? *G4doubleLeft : *intLeft ) <
                               ( G4doubleRight ? *G4doubleRight : *intRight );
                    else
                        return *intLeft < *intRight;
                case LessEq :
                    if ( isDoubleRetval )
                        return ( G4doubleLeft ? *G4doubleLeft : *intLeft ) <=
                               ( G4doubleRight ? *G4doubleRight : *intRight );
                    else
                        return *intLeft <= *intRight;
                case More :
                    if ( isDoubleRetval )
                        return ( G4doubleLeft ? *G4doubleLeft : *intLeft ) >
                               ( G4doubleRight ? *G4doubleRight : *intRight );
                    else
                        return *intLeft > *intRight;
                case MoreEq :
                    if ( isDoubleRetval )
                        return ( G4doubleLeft ? *G4doubleLeft : *intLeft ) >=
                               ( G4doubleRight ? *G4doubleRight : *intRight );
                    else
                        return *intLeft >= *intRight;
                case Eq :
                    if ( isDoubleRetval )
                        return ( G4doubleLeft ? *G4doubleLeft : *intLeft ) ==
                               ( G4doubleRight ? *G4doubleRight : *intRight );
                    else
                        return *intLeft == *intRight;
                case NotEq :
                    if ( isDoubleRetval )
                        return ( G4doubleLeft ? *G4doubleLeft : *intLeft ) !=
                               ( G4doubleRight ? *G4doubleRight : *intRight );
                    else
                        return *intLeft != *intRight;
                case And :
                    if ( G4doubleLeft )
                    {
                        if ( ! *G4doubleLeft )
                            return 0;
                    }
                    else
                    {
                        if ( ! *intLeft )
                            return 0;
                    }
                    right = GetScalarValue( ast->children[ 1 ] );
                    intRight = boost::get< int >( &right );
                    if ( ! intRight )
                    {
                        G4doubleRight = boost::get< G4double >( &right );
                        if ( ! G4doubleRight )
                            throw CexmcException( CexmcCFUnexpectedContext );
                    }
                    if ( G4doubleRight )
                    {
                        if ( *G4doubleRight )
                            return 1;
                    }
                    else
                    {
                        if ( *intRight )
                            return 1;
                    }
                    return 0;
                case Or :
                    if ( G4doubleLeft )
                    {
                        if ( *G4doubleLeft )
                            return 1;
                    }
                    else
                    {
                        if ( *intLeft )
                            return 1;
                    }
                    right = GetScalarValue( ast->children[ 1 ] );
                    intRight = boost::get< int >( &right );
                    if ( ! intRight )
                    {
                        G4doubleRight = boost::get< G4double >( &right );
                        if ( ! G4doubleRight )
                            throw CexmcException( CexmcCFUnexpectedContext );
                    }
                    if ( G4doubleRight )
                    {
                        if ( *G4doubleRight )
                            return 1;
                    }
                    else
                    {
                        if ( *intRight )
                            return 1;
                    }
                    return 0;
                default :
                    return 0;
                }
            }
            else
            {
                return GetFunScalarValue( *ast );
            }
        }
        else
        {
            const Leaf &      leaf( boost::get< Leaf >( node ) );
            const Constant *  constant( boost::get< Constant >( &leaf ) );

            if ( constant )
            {
                return *constant;
            }
            else
            {
                const Variable &  variable( boost::get< Variable >( leaf ) );
                return GetVarScalarValue( variable );
            }
        }

        return 0;
    }


    BasicEval::ScalarValueType  BasicEval::GetFunScalarValue(
                                                    const Subtree &  ast ) const
    {
        bool             evalResult( false );
        ScalarValueType  result( GetBasicFunScalarValue( ast, evalResult ) );

        if ( evalResult )
            return result;

        throw CexmcException( CexmcCFUnexpectedFunction );

        return 0;
    }


    BasicEval::ScalarValueType  BasicEval::GetVarScalarValue(
                                                    const Variable & ) const
    {
        throw CexmcException( CexmcCFUnexpectedVariable );

        return 0;
    }


    BasicEval::ScalarValueType  BasicEval::GetBasicFunScalarValue(
                                    const Subtree &  ast, bool &  result ) const
    {
        const Function &  fun( boost::get< Function >( ast.type ) );

        result = true;

        ScalarValueType   arg( GetScalarValue( ast.children[ 0 ] ) );
        int *             intArg( NULL );
        G4double *          G4doubleArg( NULL );

        intArg = boost::get< int >( &arg );
        if ( ! intArg )
            G4doubleArg = boost::get< G4double >( &arg );

        if ( fun == "Sqr" )
        {
            if ( G4doubleArg )
                return *G4doubleArg * *G4doubleArg;
            else
                return *intArg * *intArg;
        }
        if ( fun == "Sqrt" )
        {
            if ( G4doubleArg )
                return std::sqrt( *G4doubleArg );
            else
                return std::sqrt( *intArg );
        }

        result = false;

        return 0;
    }
}

#endif

