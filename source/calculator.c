#include <stdlib.h>/*malloc*/
#include <string.h>/*strlen*/
#include <assert.h>/*assert*/
#include <math.h>/*pow*/
#include "calculator.h"
#include "stack.h" 

#define NUM_OF_STATS (3)
#define NUM_OF_ASCII (128)


/*========================  typdef  =========================================*/

typedef struct error error_t;
typedef struct get_num get_num_t;
typedef struct get_opr get_opr_t;
typedef struct calculator calculator_t;
typedef  calculator_status_t (*handler_func_t) (calculator_t *calculator, char **expression);
typedef  double (*operat_func_t)(double num1, double num2);
typedef  calculator_status_t (error_func_t) (calculator_t *calculator);


/*====================== Enums ==============================================*/

typedef enum 
{
    GET_NUM ,
	GET_OPR ,
	ERROR_ST
	
}state_t;

typedef enum
{
	END_OF_CALC = -1,
	OPEN_PAR = 0,
	PLUS = 1,	
	SUBTRACTION = 1,
	MULTIPLICATION = 2,
	DIVISION = 2,
	POW_ = 3,
	CLOSE_PAR = 4
} priority_t;


/*========================== calculator struct ===============================*/

struct calculator
{
	stack_ptr_t num_stack;
	stack_ptr_t opr_stack;
	state_t state;
	handler_func_t lut_table[NUM_OF_ASCII][NUM_OF_STATS];
	priority_t priority_table[128];
	operat_func_t operator_table[128];
	double result;
};	

/*============================================================================*/

/*===============================function decleration ========================*/

static calculator_t *InitCalculator(int len);
static calculator_t *InitStructs(size_t len);
static void InitLutTable(calculator_t *calculator);
static void InitOpertorTable(calculator_t *calculator);
static void InitPriorityTable(calculator_t *calculator);
static calculator_status_t GetNumber(calculator_t *calculator, char **expression);
static calculator_status_t GetOperation(calculator_t *calculator, char **expression);
static calculator_status_t TrimWhiteSpace(calculator_t *calculator, char **expression);
static calculator_status_t OpenPar(calculator_t *calculator, char **expression);
static calculator_status_t ClosePar(calculator_t *calculator, char **expression);
static calculator_status_t CalcTwoNumbers(calculator_t *calculator);
static calculator_status_t Error(calculator_t *calculator, char **expression);
static calculator_status_t EndOfCalc(calculator_t *calculator, char **expression);
static void CalculatorDistroy(calculator_t *calculator);
static double Plus(double num1,  double num2);
static double Division(double num1,  double num2);
static double Multiplication(double num1,  double num2);
static double Subtraction(double num1,  double num2);
static double Pow(double num1,  double num2);

/*============================================================================*/

/*========================= Calculator =======================================*/

calculator_status_t Calculate(const char *expression, double *res)
{
	calculator_status_t status = SUCCESS;
	calculator_t *calculator = NULL;
	*res = 0;
	
	calculator = InitCalculator(strlen(expression));
	if(NULL == calculator)
	{
		return ERROR;
	}
	
	while('\0' != *expression)
	{	
		status = calculator->lut_table[*expression][calculator->state](calculator, (char **)&expression);
		if (SUCCESS != status)
		{	
			return status ;
		}	
		
	}

	status = EndOfCalc(calculator, (char **)&expression);
	if ('(' == *(char *)StackPeek(calculator->opr_stack))
	{
		status = SYNTAX_ERROR;
	}
	if (SUCCESS != status)
	{	
		return status ;
	}	

	*res = calculator->result;
	CalculatorDistroy(calculator);
	return status;
}

/*============================================================================*/

/*========================= Init functions and tables ========================*/

static calculator_t *InitCalculator(int len)
{
	calculator_t *calculator = NULL ;
	calculator = InitStructs(len);
	if(NULL == calculator)
	{
		return NULL;
	}
	
	InitLutTable(calculator);
	InitPriorityTable(calculator);
	InitOpertorTable(calculator);
	calculator->state = GET_NUM;
	return calculator ;
}

static calculator_t *InitStructs(size_t len)
{

	calculator_t *calculator = NULL;

	calculator = (calculator_t *)malloc(sizeof(calculator_t));
	if(NULL == calculator)
	{
		return NULL;
	}
	
	calculator->num_stack = StackCreate(len , sizeof(double));
	if(NULL == calculator->num_stack)
	{
		free(calculator);
		return NULL;
	}
	calculator->opr_stack = StackCreate(len , sizeof(char));
	if(NULL == calculator->opr_stack)
	{
		free(calculator->num_stack);
		free(calculator);
		return NULL;
	}

	calculator->state = GET_NUM;
	calculator->result = 0;
	
	return calculator;
}

static void InitLutTable(calculator_t *calculator)
{
	size_t i = 0;
	size_t j = 0;
	
	assert(NULL != calculator);
	
	for(i = 0; i < NUM_OF_ASCII; ++i )
	{
		for(j = 0; j < NUM_OF_STATS; ++j)
		{
			calculator->lut_table[i][j] = Error;
		}	
	}
	for(i = '0'; i <= '9'; ++i )
	{
		calculator->lut_table[i][GET_NUM] = GetNumber;
	}
	
	calculator->lut_table['('][GET_NUM] = OpenPar;
	calculator->lut_table['+'][GET_NUM] = GetNumber;
	calculator->lut_table['-'][GET_NUM] = GetNumber;
	calculator->lut_table[' '][GET_NUM] = TrimWhiteSpace;
	calculator->lut_table['\0'][GET_NUM] = EndOfCalc;
	
	calculator->lut_table[')'][GET_OPR] = ClosePar;
	calculator->lut_table['*'][GET_OPR] = GetOperation;
	calculator->lut_table['/'][GET_OPR] = GetOperation;
	calculator->lut_table['-'][GET_OPR] = GetOperation;
	calculator->lut_table['+'][GET_OPR] = GetOperation;
	calculator->lut_table['^'][GET_OPR] = GetOperation;
	calculator->lut_table[' '][GET_OPR] = TrimWhiteSpace;
	calculator->lut_table['\0'][GET_OPR] = EndOfCalc;	
}

static void InitPriorityTable(calculator_t *calculator)
{
	calculator->priority_table['\0'] = END_OF_CALC;
	calculator->priority_table['('] = OPEN_PAR;
	calculator->priority_table['+'] = PLUS;
	calculator->priority_table['-'] = SUBTRACTION;
	calculator->priority_table['*'] = MULTIPLICATION;
	calculator->priority_table['/'] = DIVISION;
	calculator->priority_table['^'] = POW_;
	calculator->priority_table[')'] = CLOSE_PAR;
}

static void InitOpertorTable(calculator_t *calculator)
{
	calculator->operator_table['+'] = Plus;
	calculator->operator_table['-'] = Subtraction;
	calculator->operator_table['*'] = Multiplication;
	calculator->operator_table['/'] = Division;
	calculator->operator_table['^'] = Pow;

}

/*============================================================================*/

/*============ Calculator Functionality and operations =======================*/

static calculator_status_t GetNumber(calculator_t *calculator, char **expression)
{
	char *return_ptr = NULL;
	double num = 0;
	
	assert(NULL != calculator);
	assert(NULL != expression);
	assert(NULL != *expression);
	
	num = strtod(*expression, &return_ptr);
	
	if(*expression == return_ptr)
	{
		return SYNTAX_ERROR;
	}
	
	StackPush(calculator->num_stack, (void *)&num);
	
	*expression = return_ptr;
	
	calculator->state = GET_OPR;
	
	return SUCCESS;
}

static calculator_status_t OpenPar(calculator_t *calculator, char **expression)
{
	StackPush(calculator->opr_stack, (void *)*expression);
	++*expression;
	return SUCCESS;
}

static calculator_status_t ClosePar(calculator_t *calculator, char **expression)
{
	char stack_operator = 0;
	int status = SUCCESS;
	calculator->state = GET_OPR;
	while(!StackIsEmpty(calculator->opr_stack))
	{		
		status = CalcTwoNumbers(calculator);
		if( '(' == *(char *)StackPeek(calculator->opr_stack))
		{
			StackPop(calculator->opr_stack);
			++*expression;
			return status;
		}
	}
	return SYNTAX_ERROR;
}

static calculator_status_t GetOperation(calculator_t *calculator, char **expression)
{
	double result = 0;
	double num1 = 0;
	double num2 = 0;
	char operator = 0;
	char stack_operator = 0;
	int status = SUCCESS;
	assert(NULL != calculator);
	assert(NULL != expression);
	
	operator = **expression;

	if(StackIsEmpty(calculator->opr_stack))
	{
		StackPush(calculator->opr_stack, (void *)*expression);
	}
	else
	{
		stack_operator = *(char *)StackPeek(calculator->opr_stack);
		while(calculator->priority_table[operator] <= calculator->priority_table[stack_operator] && ( !StackIsEmpty(calculator->opr_stack )) &&  2 <= StackGetSize(calculator->num_stack) && '(' != *(char *)StackPeek(calculator->opr_stack))
		{
			status = CalcTwoNumbers(calculator);	
		}	
			
		StackPush(calculator->opr_stack, (void *)*expression);
	}

	++*expression;

	calculator->state = GET_NUM;
	return status;
}

static calculator_status_t CalcTwoNumbers(calculator_t *calculator)
{
	double result = 0;
	double num1 = 0;
	double num2 = 0;
	char stack_operator = 0;
	if(StackIsEmpty(calculator->opr_stack))
	{
		return SUCCESS;
	}
	if('(' == *(char *)StackPeek(calculator->opr_stack))
	{	
		return SUCCESS;
	}
	stack_operator = *(char *)StackPop(calculator->opr_stack);
	num2 = *(double *)StackPop(calculator->num_stack);
	num1 = *(double *)StackPop(calculator->num_stack);
	if('/' == stack_operator && 0 == num2)
	{
		return MATH_ERROR;
	}
	result = calculator->operator_table[stack_operator](num1, num2);
	StackPush(calculator->num_stack, (void *)&result);
	return SUCCESS;
}

static calculator_status_t TrimWhiteSpace(calculator_t *calculator, char **expression)
{
	++*expression;
	return SUCCESS;
}

static calculator_status_t EndOfCalc(calculator_t *calculator, char **expression)
{
	int status = SUCCESS;
	if(GET_NUM == calculator->state)
	{
		return SYNTAX_ERROR;
	}
	
	status = GetOperation(calculator, expression);
	
	if(SUCCESS != status)
	{
		return status;
	}
	
	StackPop(calculator->opr_stack);
	
	calculator->result = *(double *)StackPop(calculator->num_stack);
	return SUCCESS;
}

static calculator_status_t Error(calculator_t *calculator, char **expression)
{
	assert(NULL != calculator);
	assert(NULL != expression);
	
	return ERROR;
}

static void CalculatorDistroy(calculator_t *calculator)
{
	StackDestroy(calculator->opr_stack);
	StackDestroy(calculator->num_stack);
	free(calculator);
}

/*===================== Mathematical operations ============================*/

static double Plus(double num1,  double num2)
{
	return (num1 + num2);
} 

static double Subtraction(double num1,  double num2)
{
	return (num1 - num2);
}

static double Multiplication(double num1,  double num2)
{
	return (num1 * num2);
}
	
static double Division(double num1,  double num2)
{
	return (num1 / num2);
}

static double Pow(double num1,  double num2)
{	
	return pow(num1, num2);
}

/*============================================================================*/







