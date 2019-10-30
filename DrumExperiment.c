#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/** maximum length of the line*/
#define LINE_MAX_SIZE 1024
/** possible inputs to ignore using strtok*/
#define INPUT_DELIMS " \t\r\n"
/** maximum elements allowed*/
#define MAX_ELEMENTS 100
/**error printed in case of an illegal input*/
#define ERROR fprintf(stderr, "ERROR\n"); return EXIT_FAILURE;
/**range of convolution*/
#define N_FLOOR MAX_ELEMENTS/2
/**all digits*/
#define ASCII_NUMS "0123456789"
/**backspace print*/
#define BACKSPACE_PRINT "\n"
/**ast*/
#define AST "*"

/**decimal base is 10*/
const int DEC_BASE = 10;
/**first number is ascii table*/
const char DIGIT_ASCII_BEGIN = '0';
/**false*/
const int FALSE = 0;
/**true*/
const int TRUE = 1;
/**representing illegal input*/
const int ILLEGAL = -1;
/**maximum digits in a double allowed*/
const unsigned int DOUBLE_MAX_SIZE = 9;
/**decimal point*/
const char DEC_POINT = '.';
/**minimum of the array's maximum element*/
const double EPSILON = 0.00000001;
/**in order to round to the 3 digit after the decimal point*/
const int THREE_DIGIT_ROUND = 1000;
/**maximum number of asterisk*/
const int MAX_AST = 20;
/**end of the array*/
const char ARR_END = '\0';

/**
 * This function will receive a number in a form of string, without delimeters, and will
 * return it's double representation.
 * @param value pointer to an array of chars
 * @return the char received as a double if legal, -1 otherwise
 */
double readRationalNumber(char *value)
{
    double completePart = 0, fractionalPart = 0;
    int fracFlag = FALSE;
    double divider = 1.0; /*the divider for the fractional part*/
    /* if not a digit*/
    if (!strchr(ASCII_NUMS, value[0]))
    {
        return ILLEGAL;
    }
    /*if first digit is 0 and second is not end of array*/
    if (value[0] == DIGIT_ASCII_BEGIN && value[1] != ARR_END)
    {
        /* if there's not decimal point after it, is illegal input*/
        if (value[1] != DEC_POINT)
        {
            return ILLEGAL;
        }
    }
    /* running over the string*/
    while (*value != ARR_END)
    {
        /* dereference next digit as a char*/
        char cur = *value;
        if (cur == DEC_POINT)
        {
            /* if a dot has already been found, illegal input*/
            if (fracFlag == TRUE)
            {
                return ILLEGAL;
            }
            fracFlag = TRUE;
        }
        else
        {
            /* if it's not a digit*/
            if (!strchr(ASCII_NUMS, cur))
            {
                return ILLEGAL;
            }
            /* if we are still in the left part of the decimal dot*/
            if (fracFlag == FALSE)
            {
                completePart = completePart * DEC_BASE + (cur - DIGIT_ASCII_BEGIN);
            }
                /* if we have passed the dot*/
            else
            {
                fractionalPart += (cur - DIGIT_ASCII_BEGIN) / (DEC_BASE * divider);
                divider *= DEC_BASE;
            }
        }
        /* pointer forwarding*/
        value++;
    }
    return completePart + fractionalPart;
}

/**
 * This function will receive the third line in order to read a natural number
 * @param list the input from the user
 * @return the natural number if legal, -1 otherwise
 */
int readN(char *list)
{
    int roomNum = 0;
    size_t len = strlen(list);
    /*if nothing was entered*/
    if (len == 1 && !strchr(ASCII_NUMS, *list))
    {
        return ILLEGAL;
    }
    for (unsigned long i = 0; i < len; i++)
    {
        if (!strchr(ASCII_NUMS, *list) && i != len - 1)
        {
            return ILLEGAL;
        }
        else
        {
            roomNum = roomNum * DEC_BASE + (*list - DIGIT_ASCII_BEGIN);
        }
        list++;
    }
    return roomNum;

}

/**
 * This function will be used to read first two lines, and create an array of doubles
 * out of a string
 * @param list a line received as an input
 * @return number of elements received, -1 if illegal
 */
int readArr(double *list)
{
    int itemsCounter = 0;
    char line[LINE_MAX_SIZE];
    char *value = NULL;
    if (fgets(line, LINE_MAX_SIZE, stdin) == NULL)
    {
        return ILLEGAL;
    }
    value = strtok(line, INPUT_DELIMS);
    while (value != NULL)
    {
        itemsCounter++;
        if (itemsCounter > MAX_ELEMENTS || strlen(value) > DOUBLE_MAX_SIZE ||
            (*list = readRationalNumber(value)) < 0)
        {
            return ILLEGAL;
        }
        list++;
        value = strtok(NULL, INPUT_DELIMS);

    }
    return itemsCounter;
}

/**
 * This function will center the elements in the array
 * @param list to center
 * @param numOfElements number of elements in the array
 */
void centerArr(double *list, int numOfElements)
{
    int offset = (MAX_ELEMENTS - numOfElements) / 2;
    /* zeros on the right of the array*/
    for (int i = MAX_ELEMENTS - 1; i > MAX_ELEMENTS - offset - 1; i--)
    {
        list[i] = 0;
    }
    /*taking all elements to their correct center place*/
    for (int i = MAX_ELEMENTS - offset - 1; i > offset - 1; i--)
    {
        list[i] = list[i - offset];
    }
    /* zeros on the left of the array*/
    for (int i = offset - 1; i >= 0; i--)
    {
        list[i] = 0;
    }
}

/**
 * This function will normalize elements in the array, so that they sum to 1
 * @param list to normalize
 */
void normalizeArr(double *list)
{
    double sumOfElements = 0;
    for (int i = 0; i < MAX_ELEMENTS; i++)
    {
        sumOfElements += list[i];
    }
    /*division in 0.000000 will cause -nan result*/
    if (sumOfElements > 0)
    {
        for (int i = 0; i < MAX_ELEMENTS; i++)
        {
            list[i] /= sumOfElements;
        }
    }
}

/**
 * This function does g*h and put the result of the convolution in the result
 * @param g first array
 * @param h second array
 * @param result array for result
 */
void convolution(const double *g, const double *h, double *result)
{
    for (int t = -N_FLOOR - 1; t < N_FLOOR - 1; t++)
    {
        double localSum = 0;
        for (int m = -N_FLOOR; m < N_FLOOR; m++)
        {
            if (MAX_ELEMENTS >= (t - m + N_FLOOR) && (t - m + N_FLOOR) >= 0 &&
                MAX_ELEMENTS >= m + N_FLOOR && m + N_FLOOR >= 0)
            {
                localSum += g[t - m + N_FLOOR] * h[m + N_FLOOR];
            }
        }
        result[t + N_FLOOR + 1] = localSum;
    }
}

void printList(double *list)
{
    for (int i = 0; i < MAX_ELEMENTS; i++)
    {
        printf("%f, ", list[i]);
    }
    printf("\n");
}

/**
 * This function takes an array and round the number to 3 digits after the decimal points
 * @param list to round
 */
void roundArr(double *list)
{
    for (int i = 0; i < MAX_ELEMENTS; i++)
    {
        list[i] = (round(THREE_DIGIT_ROUND * list[i])) / THREE_DIGIT_ROUND;
    }
}

/**
 * This function finds the maximum value of the array
 * @param list to find max in
 * @return the maximum
 */
double findMax(const double *list)
{
    double localMax = list[0];
    for (int i = 1; i < MAX_ELEMENTS; i++)
    {
        if (localMax < list[i])
        {
            localMax = list[i];
        }
    }
    return localMax;
}

/**
 * This function swaps two arrays
 * @param pa pointer to first array
 * @param pb pointer to second array
 */
void swap(double *pa, double *pb)
{
    for (int i = 0; i < MAX_ELEMENTS; i++)
    {
        double temp = pa[i];
        pa[i] = pb[i];
        pb[i] = temp;
    }
}

/**
 * This main function will use other functions to receive 2 arrays with the measurements,
 * an integer representing the number of rooms in the experiment, and after centering, normalizing
 * and making convolution as many times as the number of rooms, and prints asterisk representing
 * the sound
 * @return 0 upon success
 */
int main()
{
    char nLine[LINE_MAX_SIZE];
    double h[MAX_ELEMENTS] = {0}, g[MAX_ELEMENTS] = {0}, gConvH[MAX_ELEMENTS] = {0};
    int hSize, gSize;
    /*receiving arrays and checking for illegal input*/
    if ((gSize = readArr(g)) == ILLEGAL || gSize == 0 || (hSize = readArr(h)) == ILLEGAL ||
        hSize == 0 || hSize > gSize)
    {
        ERROR
    }
    /*getting number of rooms*/
    if (fgets(nLine, LINE_MAX_SIZE, stdin) == NULL)
    {
        return ILLEGAL;
    }
    /*analyzing roomNum*/
    int roomNum = readN(nLine);
    if (roomNum == ILLEGAL)
    {
        ERROR
    }
    centerArr(g, gSize);
    centerArr(h, hSize);
    normalizeArr(g);
    normalizeArr(h);
    /*if no rooms, the result is simply g (no convolution needed)*/
    if (roomNum == 0)
    {
        swap(g, gConvH);
    }
    else
    {
        /*first convolution*/
        convolution(g, h, gConvH);
        normalizeArr(gConvH);
    }
    /*if there's more then 1 room, keep convolution and swap arrays to do the right conv*/
    for (int i = 1; i < roomNum; i++)
    {
        convolution(gConvH, h, g);
        swap(gConvH, g);
        normalizeArr(gConvH);
    }
    roundArr(gConvH);
    double maximumValue = findMax(gConvH);
    /*if the max value is not high enough, finish */
    if (maximumValue <= EPSILON)
    {
        return 0;
    }
    /*final print of the output*/

    for (int i = 0; i < MAX_ELEMENTS; i++)
    {
        int numOfStars = floor((gConvH[i] * MAX_AST) / maximumValue);
        printf("%0.3f: ", gConvH[i]);
        for (int j = 0; j < numOfStars; j++)
        {
            printf(AST);
        }
        if (i != MAX_ELEMENTS - 1)
        {
            printf(BACKSPACE_PRINT);
        }
    }
    return 0;
}

