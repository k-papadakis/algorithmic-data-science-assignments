/***************************************************************************
                          apriori.cpp  -  description
                             -------------------
    begin                : cs dec 26 2002
    copyright            : (C) 2002 by Ferenc Bodon
    email                : bodon@mit.bme.hu
 ***************************************************************************/

#include "Apriori.hpp"
#include <iostream>
#include <vector>
#include <set>
#include <cmath>   //because of the ceil function

using namespace std;

/**
  \param candidate_size The size of the candidate whose support 
                        has top be determined.
*/
void Apriori::support( const itemtype& candidate_size )
{
   vector<itemtype> basket, basket_v;
   if (candidate_size == 2)
   {
      while( input_output_manager.read_in_a_line( basket ) )
      {
	 input_output_manager.basket_recode( basket, basket_v );
	 if (basket_v.size()>1) reduced_baskets[basket_v]++;
      }
   }
   for (map<vector<itemtype>,unsigned long>::iterator it = 
	   reduced_baskets.begin(); it!=reduced_baskets.end();it++)
      apriori_trie->find_candidate(it->first,candidate_size,it->second);

}
/**
  \param basket_file The file that contain the transactions.
  \param output_file_name The name of file where the results have to be 
                          written to.
*/
Apriori::Apriori( ifstream& basket_file, const char*  output_file_name ):
                  input_output_manager(basket_file, output_file_name )
{
}

/**
  \param min_supp The relative support threshold
*/
void Apriori::APRIORI_alg( const unsigned long min_supp )
{
   unsigned long basket_number;   
   itemtype candidate_size=1;
   vector<unsigned long> support_of_items;
   basket_number = input_output_manager.find_frequent_items( 
      min_supp, support_of_items );
   input_output_manager<< " ("<< basket_number << ')' << endl;
   set<itemtype> temp_set;
   for(vector<unsigned long>::size_type index = 0;
      index < support_of_items.size(); index++)
   {
      temp_set.insert(index);
      input_output_manager.write_out_basket_and_counter( temp_set, support_of_items[index] );
      temp_set.erase(temp_set.begin());
   }
   apriori_trie = new Apriori_Trie( basket_number );
   apriori_trie->insert_frequent_items( support_of_items );

   candidate_size++;
   apriori_trie->candidate_generation(candidate_size-1, input_output_manager);
   while( apriori_trie->is_there_any_candidate() )
   {
      input_output_manager.rewind();
      support( candidate_size );
      apriori_trie->delete_infrequent(min_supp, candidate_size);
      candidate_size++;
      apriori_trie->candidate_generation(candidate_size-1, 
					 input_output_manager);
   }
   apriori_trie->statistics();
}

Apriori::~Apriori()
{
   delete apriori_trie;
}
