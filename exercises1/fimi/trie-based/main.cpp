/** \mainpage An efficient implemenation of APRIORI algorithm

This program is a very efficient implementation of APRIORI algorithm
proposed by Rakesh Agrawal and Ramakrishnan Srikant.  APRIORI is the
most basic and well-known algorithm to find frequent itemsets in a
transactional database.

<h2>Frequent Itemset Mining problem</h2> A <i>transactional
database</i> consists of sequence of transaction: \f$T=\langle
t_1,\ldots ,t_n\rangle \f$.  A transaction is a set of items
(\f$t_i\in I\f$).  Transactions are often called <i>baskets</i>,
referring to the primary application domain (i.e. market-basket
analysis).  A set of items is often called <i>itemset</i> by the data
mining community.  The <i>(absolute) support</i> or the
<em>occurrence</em> of \f$X\f$ (denoted by \f$supp(X)\f$) is the
number of transactions that are supersets of \f$X\f$ (i.e. that
<em>contain</em> \f$X\f$).  The <em>realtive support</em> is the
absolute support divided by the number of transactions
(i.e. <i>n</i>).  An itemset is <i>frequent</i> if its support is
greater or equal than a threshold value.

<p>
In the frequent itemset mining problem a transaction database and a relative support threshold (traditionally denoted by <i>min_supp</i>) is given and we have to find all frequent itemsets.
</p>

 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <fstream>
#include "Apriori.hpp"
using namespace std;


/// This procedure displays the usage of the program.

void usage()
{
   cerr << "\nUsage: fim_all basketfile min_supp outcomefile \n";
   cerr << "\n basketfile\t    file, that contains the baskets of itemcodes";
   cerr << "\n min_supp\t    support threshold";
   cerr << "\n outcomefile\t    file to write the outcome";

   cerr << "\n\nFile formats:";
   cerr << "\n\nThe basket file is a plan text file. Each row represents a basket. ";
   cerr << "A basket is a set of items seperated by a nonnumeric character (for example white space, comma, colon, etc.). ";
   cerr << "An item is represented by its code which is an integer number greater than or equal to 0.";
   cerr << "\n\nHave a succesful mining ;-)";
   cerr << "\n\n\n\t\t\t\t\tFerenc Bodon\n\n";
}

/// This procedure processes the arguments.
int process_arguments( int argc, char *argv[], ifstream& basket_file, 
		       unsigned long& min_supp )
{
   if ( argc < 4 )
   {
     usage();
     cerr<<"\nError! There are 3 mandatory arguments!"<<endl;
     return 1;
   }
   basket_file.open(argv[1]);
   if( !basket_file )
   {
      usage();
      cerr << "\nError! The basket file can not be read!"<< endl;
      return 1;
   }
   char* pEnd;
   min_supp = strtoul( argv[2], &pEnd, 0 );
   return 0;
}

int main( int argc, char *argv[] )
{
   unsigned long min_supp;
   ifstream basket_file;
   
   if( process_arguments( argc, argv, basket_file, min_supp ) ) return 1;
        
   Apriori apriori( basket_file, argv[3] );
   apriori.APRIORI_alg( min_supp );
   basket_file.close();
   return 0;
}

