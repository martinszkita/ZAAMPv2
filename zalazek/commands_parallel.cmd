#define ROTATE_SPEED 30
/*
* Przykaadowy zestaw polece«
*/
Begin_Parallel_Actions
    Set Podstawa.Ramie1 2 0 0 30 0 0 
    Set Podstawa.Ramie1.Ramie2 10 10 5 0 0 0 
End_Parallel_Actions
Begin_Parallel_Actions
Rotate Podstawa X ROTATE_SPEED 40
End_Parallel_Actions
Begin_Parallel_Actions
 Pause 1000 /* Zawieszenie na 1 sek. */ 
End_Parallel_Actions
Begin_Parallel_Actions
Move Podstawa2 10 10
Rotate Podstawa2.Ramie21.Ramie22 ROTATE_SPEED Z 60 /* Rotate i Move wykonywane razem */
Move Podstawa2.Ramie21 10 20
 /* powoduja jazde po luku
 */
End_Parallel_Actions