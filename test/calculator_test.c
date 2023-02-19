#include "calculator.h"

#include "stack.h" 

int main()
{
    calculator_status_t status = SUCCESS;
    stack_ptr_t stack = NULL;
    double result = 0;
    
    char str1[100] = "7+8";        
          /* Result = 15 status = SUCCESS       */
    char str2[100] = "8+8*3+-2^5";  
           /* Result =  0 status = SUCCESS      */
    char str3[100] = "8+8*3-2^";     
        /* Result =  0 status = SYNTAX_ERROR */
    char str4[100] = "2/0";          
       /* Result =  0 status = MATH_ERROR         */
    char str5[100] = "8++8*((3-2)*5)";   
      /* Result = 48 status = SUCCESS       */
    char str6[100] = "3-2)*5";       
      /* Result =  0 status = SYNTAX_ERROR */
    char str7[100] = "(3-2)*5+ 5*(4+4+4";
        /* Result =  0 status = SYNTAX_ERROR */

	status = Calculate(str1, &result);
	printf("%s\nstatus %d result : %f\n",str1, (int)status, result);
	status = Calculate(str2, &result);
	printf("%s\nstatus %d result : %f\n",str2, (int)status, result);
	status = Calculate(str3, &result);	
	printf("%s\nstatus %d result : %f\n",str3, (int)status, result);
	status = Calculate(str4, &result);
	printf("%s\nstatus %d result : %f\n",str4, (int)status, result);
	status = Calculate(str5, &result);
	printf("%s\nstatus %d result : %f\n",str5, (int)status, result);
	status = Calculate(str6, &result);
	printf("%s\nstatus %d result : %f\n",str6, (int)status, result);
	status = Calculate(str7, &result);
	printf("%s\nstatus %d result : %f\n",str7, (int)status, result);

	return 0;
}







