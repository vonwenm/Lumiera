/*
  QUERY.hpp  -  interface for capability queries

  Copyright (C)         Lumiera.org
    2008,               Hermann Vosseler <Ichthyostega@web.de>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation; either version 2 of
  the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/


#ifndef LUMIERA_QUERY_H
#define LUMIERA_QUERY_H


#include <string>
#include <cctype>
#include <typeinfo>

#include "lib/symbol.hpp"


namespace lumiera {
  
  using lib::Symbol;
  using lib::Literal;
  using std::string;
  
  /* ==== common definitions for rule based queries ==== */
  
  
  
  /**
   * Generic query interface for retrieving objects
   * matching the given capability query
   * @note until really integrating a rules based system
   *       this is a dummy placeholder implementation
   *       based on a dressed-up std::string
   * @warning in this preliminary implementation, the
   *       given query-string is used as-is, without any
   *       normalisation. Moreover, as espeically the
   *       fake-configrules match by string comparision,
   *       this may led to unexpected mis-matches.
   * @todo the »real thing« should be based on a
   *       normalised syntax tree representation
   *       of the atoms in the query.
   */
  template<class OBJ>
  class Query
    {
       std::string predicateForm_;
       
    public:
      explicit Query (string const& predicate="")
        : predicateForm_(predicate)
        { }
      

      void
      clear()
        {
          predicateForm_.clear();
        }
      
      operator string()  const { return predicateForm_; }
      
//    operator string& () { return predicateForm_; }      //TICKET #710 : needed temporarily by fake-configrules 
    };                                                   //               for calling removeTerm on the string-ref....
  
  
  namespace query {
    
    /** ensure standard format for a given id string.
     *  Trim, sanitise and ensure the first letter is lower case.
     *  @note modifies the given string ref in place
     */
    void normaliseID (string& id);
    
    
    /** count the top-level predicates in the query string.
     *  usable for ordering queries, as more predicates usually
     *  mean more conditions, i.e. more constriction
     */
    uint countPred (const string&);
    
    
    const string extractID (Symbol, const string& termString);
    
    const string removeTerm (Symbol, string& termString);
    
    
    template<typename TY>
    const string
    buildTypeID()
    {
      string typeID (typeid(TY).name());
      normaliseID (typeID);
      return typeID;
    }
    
    
    
}} // namespace lumiera::query
#endif
