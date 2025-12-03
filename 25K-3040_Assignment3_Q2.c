#include<stdio.h>
int calculateFuel(int fuel, int consumption, int recharge, int solarBonus, int planet, int totalPlanets)
{
	if(fuel == 0 || planet > totalPlanets)
    {
        return fuel;
    }
    int newfuel = fuel - consumption;
    if(planet % 2 == 0)
    {
        newfuel += recharge;
    }
    if(planet % 4 == 0)
    {
        newfuel += solarBonus;
    }
    if(newfuel < 0)
    {
        newfuel = 0;
    }
    printf("Planet %d: Fuel Remaining = %d\n", planet, newfuel);
    return calculateFuel(newfuel, consumption, recharge, solarBonus, planet+1, totalPlanets);
}

int main()
{
	int fuel, consumption, recharge, solarBonus, planet;
	printf("Enter Initial Fuel: ");
	scanf("%d", &fuel);
	printf("Enter Fuel Consumption Per Planet: ");
	scanf("%d", &consumption);
	printf("Enter Amount of Fuel Gained in Gravitational Zones: ");
	scanf("%d", &recharge);
	printf("Enter Solar Bonus After Every 4th Planet: ");
	scanf("%d", &solarBonus);
	calculateFuel(fuel, consumption, recharge, solarBonus, 1, 10);
	return 0;
}
