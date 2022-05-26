/***************************************************************************
                          Trie.cpp  -  description
                             -------------------
    begin                : cs dec 26 2002
    copyright            : (C) 2002 by Ferenc Bodon
    email                : bodon@mit.bme.hu
 ***************************************************************************/


#include "Trie.hpp"
#include <cstdlib>
#include <iostream>
#include <algorithm>

bool Edge_point_less(const Edge edge, const itemtype label)
{
   return edge.label < label;
};

/**
  \param init_counter The initial counter of the new trie
 */
Trie::Trie( const unsigned long init_counter):
   counter(init_counter)
{
}

/**
  \param an_itemset The given itemset.
  \param item_it This iterator shows the element of the basket 
                 that has to be processed.
  \return NULL, if the itemset is not included, otherwise the trie, 
                that represents the itemset.
*/

const Trie* Trie::is_included( const set<itemtype>& an_itemset, 
			       set<itemtype>::const_iterator item_it ) const
{
   if( item_it == an_itemset.end() ) return this;
   else
   {
      vector<Edge>::const_iterator it_edgevector = 
	 lower_bound(edgevector.begin(), edgevector.end(), 
		     *item_it, Edge_point_less);
      if( it_edgevector != edgevector.end() && 
	  (*it_edgevector).label == *item_it )
         return (*it_edgevector).subtrie->is_included( an_itemset, ++item_it );
      else return NULL;
   }
}

/**
  \param it_basket_upper_bound the last item in the basket, 
                               that has to be checked
  \param distance_from_candidate The length of the path from the actual trie 
                                 to a trie that represents a candidate
  \param it_basket *it_basket lead to the actual Trie. Only items 
                   following this item in the basket need to be considered
  \param counter_incr The number times this basket occurs
*/
void Trie::find_candidate( vector<itemtype>::const_iterator it_basket_upper_bound,
			   const itemtype distance_from_candidate,
			   vector<itemtype>::const_iterator it_basket, 
			   const unsigned long counter_incr)
{
   if( distance_from_candidate )
   {
         vector<Edge>::iterator it_edge = edgevector.begin();
         while( it_edge != edgevector.end() 
		&& it_basket != it_basket_upper_bound )
         {
            if( (*it_edge).label < *it_basket) it_edge++;
            else if( (*it_edge).label > *it_basket) it_basket++;
            else
            {
                  (*it_edge).subtrie->find_candidate( it_basket_upper_bound + 1, 
						      distance_from_candidate - 1, 
						      it_basket + 1, counter_incr);
               it_edge++;
               it_basket++;
            }
	 }
   }
   else counter += counter_incr;
}

/**
  \param min_occurrence The occurence threshold
  \param distance_from_candidate_parent The length of the path from the root 
                                        of the actual tre to a root of the trie 
                                        that represents the parent of a candidate
*/
void Trie::delete_infrequent( const unsigned long min_occurrence, 
			      const itemtype distance_from_candidate_parent,
			      unsigned long& nr_of_freq_itemsets)
{
   vector<Edge>::iterator itEdge = edgevector.begin();
   if( distance_from_candidate_parent )
   {
      while(  itEdge != edgevector.end() )
      {
          (*itEdge).subtrie->delete_infrequent( min_occurrence,
						distance_from_candidate_parent - 1,
						nr_of_freq_itemsets );
	 if( (*itEdge).subtrie->edgevector.empty() )
	 {
	    delete (*itEdge).subtrie;
	    itEdge = edgevector.erase(itEdge);
	 }
	 else itEdge++;
      }
   }
   else
   {
      while ( itEdge != edgevector.end() )
      {
         if( (*itEdge).subtrie->counter < min_occurrence )
         {
            delete (*itEdge).subtrie;
            itEdge = edgevector.erase(itEdge);
         }
         else
	 {
	    itEdge++;
	    nr_of_freq_itemsets++;
	 }
      }
   }
}

Trie::~Trie()
{
   for( vector<Edge>::iterator itEdge = edgevector.begin();
	itEdge != edgevector.end(); itEdge++ )
      delete (*itEdge).subtrie;
}


/**
  \param item The label of the new edge
  \param counter The initial counter of the new state
 */
void Trie::add_empty_state( const itemtype item, const unsigned long counter )
{
   Edge temp_edge;
   temp_edge.label = item;
   temp_edge.subtrie = new Trie( counter );
   edgevector.push_back(temp_edge);
}
