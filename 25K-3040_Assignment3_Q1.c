#include<stdio.h>
float calculateRepayment(float loan, float interestRate, int years)
{
    if(loan <= 0 || years == 0)
	{
        return 0;
    }
    static int totalYears = 0;
    if(totalYears == 0)
	{
		totalYears = years;
	}
    int currentYear = totalYears - years + 1;
    float installment = 35000;
    loan += loan * interestRate;
    loan -= installment;
    if(loan < 0)
	{
		loan = 0;
	}
    printf("Year %d: Remaining Loan = %.2f\n", currentYear, loan);
    return installment + calculateRepayment(loan, interestRate, years - 1);
}

int main()
{
    float loan, interestRate;
    int years;
    printf("Enter the Total Loan: ");
    scanf("%f", &loan);
    printf("Enter the Annual Interest Rate (%%): ");
    scanf("%f", &interestRate);
    interestRate /= 100;
    printf("Enter the No of Years in Which the Loan has to be Repaid: ");
    scanf("%d", &years);
    float total = calculateRepayment(loan, interestRate, years);
    printf("Total repayment over %d years: %.2f\n", years, total);
    return 0;
}

