/***************************************************************************
                          Apriori_Trie.cpp  -  description
                             -------------------
    begin                : cs dec 26 2002
    copyright            : (C) 2002 by Ferenc Bodon
    email                : bodon@mit.bme.hu
 ***************************************************************************/

#include <cstdlib>
#include <iostream>
#include <iterator>
#include <numeric>
#include "Apriori_Trie.hpp"

/**
  \param counter_of_emptyset The support of the empty set, 
                             i.e. the number of transactions.
*/
Apriori_Trie::Apriori_Trie(const unsigned long counter_of_emptyset ):
   main_trie(counter_of_emptyset)
{
   nr_of_freq_itemsets.push_back(1);
}

/**
  \param counters It stores the support of the items.
                  counters[i] stores the suport of item i.
*/
void Apriori_Trie::insert_frequent_items(
   const vector<unsigned long>& counters )
{
   for(vector<unsigned long>::size_type item_index = 0; 
       item_index < counters.size(); item_index++)
      main_trie.add_empty_state( item_index, counters[item_index] );
   nr_of_freq_itemsets.push_back(counters.size());
}

/**
  \param frequent_size Size of the frequent itemsets that 
                       generate the candidates.
*/
void Apriori_Trie::candidate_generation( const itemtype& frequent_size,
					 Input_Output_Manager& 
					 input_output_manager )
{
   if( frequent_size == 1 ) candidate_generation_two();
   else
   {
      set<itemtype> maybe_candidate;
      candidate_generation_assist( &main_trie, frequent_size-1, 
				   maybe_candidate, input_output_manager );
   }
}

/**
  \param basket The basket that hs to be analyzed.
  \param candidate_size the size of the candidates.
  \param counter_incr The number of time the basket occured.
                      The counters of candidates that occure in the basket has 
		      to be incremented by counter_incr.
*/
void Apriori_Trie::find_candidate( const vector<itemtype>& basket, 
				   const itemtype candidate_size, 
				   const unsigned long counter_incr)
{
   if( candidate_size != 2 ) 
      if ( candidate_size<basket.size()+1 )
	 main_trie.find_candidate( basket.end()-candidate_size+1, 
				   candidate_size, 
				   basket.begin(), counter_incr );
      else;
   else find_candidate_two( basket, counter_incr );    
}

/**
  \param min_occurrence The threshold of absolute support.
  \param candidate_size The size of the candidate itemset.
*/
void Apriori_Trie::delete_infrequent( const unsigned long min_occurrence, 
				      const itemtype candidate_size )
{
   nr_of_freq_itemsets.push_back(0);
   if( candidate_size != 2 ) 
      main_trie.delete_infrequent( min_occurrence, 
				   candidate_size - 1,
				   nr_of_freq_itemsets[candidate_size]);
   else delete_infrequent_two( min_occurrence );
   if( !nr_of_freq_itemsets.back() ) 
      nr_of_freq_itemsets.pop_back();
}

bool Apriori_Trie::is_there_any_candidate() const
{
   return !main_trie.edgevector.empty();
}

void Apriori_Trie::statistics() const
{
   cout << accumulate(nr_of_freq_itemsets.begin(), 
		      nr_of_freq_itemsets.end(), 0) << '\n';
   copy(nr_of_freq_itemsets.begin(), 
	nr_of_freq_itemsets.end(), 
	ostream_iterator<unsigned long>(cout, "\n"));
}

Apriori_Trie::~Apriori_Trie()
{
}

/**
  \param maybe_candidate The itemset that has to be checked.
 */

bool Apriori_Trie::is_all_subset_frequent( 
   const set<itemtype>& maybe_candidate ) const
{
   if( maybe_candidate.size() < 3) return true; // because of the 
                                                // candidate generation method!
   else
   {
      set<itemtype>                 temp_itemset(maybe_candidate);
      set<itemtype>::const_iterator item_it = --(--maybe_candidate.end());
      do
      {
         item_it--;
         temp_itemset.erase( *item_it );
         if( !main_trie.is_included( temp_itemset, temp_itemset.begin() ) ) 
	    return false;
         temp_itemset.insert( *item_it );
      }
      while ( item_it != maybe_candidate.begin() );
      return true;
   }
}

void Apriori_Trie::candidate_generation_two()
{
   if( !main_trie.edgevector.empty() )
   {
      temp_counter_array.reserve(main_trie.edgevector.size()-1);
      temp_counter_array.resize(main_trie.edgevector.size()-1);
      for( vector<Edge>::size_type stateIndex = 0; 
	   stateIndex < main_trie.edgevector.size()-1; stateIndex++ )
      {
         temp_counter_array[stateIndex].reserve(
	    main_trie.edgevector.size()-1-stateIndex );
         temp_counter_array[stateIndex].resize(
	    main_trie.edgevector.size()-1-stateIndex, 0);
      }
   }
}

void Apriori_Trie::candidate_generation_assist( 
   Trie* trie, 
   const itemtype distance_from_generator,
   set<itemtype>& maybe_candidate,
   Input_Output_Manager& input_output_manager)
{
   vector<Edge>::iterator itEdge = trie->edgevector.begin();
   if( distance_from_generator )
   {
      while( itEdge != trie->edgevector.end() )
      {
         maybe_candidate.insert((*itEdge).label);
         candidate_generation_assist(
	    (*itEdge).subtrie, distance_from_generator - 1, 
	    maybe_candidate, input_output_manager );
         maybe_candidate.erase((*itEdge).label);
 	 if((*itEdge).subtrie->edgevector.empty())
	 {
	    delete (*itEdge).subtrie;
	    itEdge = trie->edgevector.erase(itEdge);
	 }
         else itEdge++;

      }
   }
   else
   {
      vector<Edge>::iterator itEdge2;
      Trie* toExtend;
      while( itEdge != trie->edgevector.end() )
      {
         maybe_candidate.insert((*itEdge).label);
         toExtend = (*itEdge).subtrie;
	 input_output_manager.write_out_basket_and_counter( maybe_candidate, toExtend->counter );
         for( itEdge2 = itEdge + 1; 
	      itEdge2 != trie->edgevector.end(); itEdge2++ )
         {
            maybe_candidate.insert( (*itEdge2).label );
            if( is_all_subset_frequent( maybe_candidate) )
               toExtend->add_empty_state( (*itEdge2).label );
            maybe_candidate.erase( (*itEdge2).label );
         }
	 (vector<Edge>(toExtend->edgevector)).swap(toExtend->edgevector);  
          maybe_candidate.erase((*itEdge).label);
         if( toExtend->edgevector.empty())
	 {
	    delete (*itEdge).subtrie;
	    itEdge = trie->edgevector.erase(itEdge);
	 }    
	 else itEdge++;
  // we know that state toExtend will not have any more children!

      }

   }
}

/**
     \param basket the given basket
     \param counter The number the processed basket occures 
                    in the transactional database
   */

void Apriori_Trie::find_candidate_two( const vector<itemtype>& basket, 
				       const unsigned long counter )
{
   if( basket.size() > 1)
   {
      vector<itemtype>::const_iterator it1_basket,
                                       it2_basket;

      for( it1_basket = basket.begin(); it1_basket != basket.end()-1; 
	   it1_basket++)
         for( it2_basket = it1_basket+1; it2_basket != basket.end(); 
	      it2_basket++)
            temp_counter_array[*it1_basket][*it2_basket-*it1_basket-1] 
	       += counter;
   }
}

/**
  \param min_occurrence The occurence threshold
*/
void Apriori_Trie::delete_infrequent_two( const unsigned long min_occurrence )
{
   vector<Edge>::size_type stateIndex_1,
                            stateIndex_2;
   for( stateIndex_1 = 0; stateIndex_1 < main_trie.edgevector.size()-1; 
	stateIndex_1++ )
   {
      for( stateIndex_2 = 0; 
	   stateIndex_2 < main_trie.edgevector.size() - 1 - stateIndex_1; 
	   stateIndex_2++ )
      {
        if( temp_counter_array[stateIndex_1][stateIndex_2] >= min_occurrence )
	{
           main_trie.edgevector[stateIndex_1].subtrie->add_empty_state( 
	      stateIndex_1 + stateIndex_2 + 1,
	      temp_counter_array[stateIndex_1][stateIndex_2] );
	   nr_of_freq_itemsets[2]++;
	}
      }
      temp_counter_array[stateIndex_1].clear();
 /// temp_counter_array[stateIndex_1] will never be used again!
      vector<unsigned long>().swap(temp_counter_array[stateIndex_1]);  
   }
   temp_counter_array.clear();
 /// temp_counter_array will never be used again!
   vector< vector<unsigned long> >().swap(temp_counter_array);
   vector<Edge>::iterator it= main_trie.edgevector.begin();
   while( it!=main_trie.edgevector.end() )
      if((*it).subtrie->edgevector.empty())
      {
	 delete (*it).subtrie;
         it = main_trie.edgevector.erase(it);
      }
      else it++;
}
