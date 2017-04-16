#ifndef GA_h
#define GA_h

#ifndef MAX_BOARD
#define	MAX_BOARD 100
#endif

/*GA DATA*/
#ifndef individualNum
#define individualNum 32	//Number of individuals.
#endif
#ifndef interationNum
#define interationNum 256	//Number of interation times.
#endif
/*GD END*/

/*GA FUNC*/
void GA();
void resizeVector_GA();	//Resize all vector
void init_GA();	//Initial.
void getValue_GA();	//Get individual's value.
void getBest_GA();	//Get the best individual.
void choose_GA();	//Spin table choose.
void recombine_GA();	//Recombine
void mutate_GA();	//Mutate
/*GAFC END*/

#endif /* GA_h */
