// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillSystem.h"

USkillSystem::USkillSystem()
{
	for (int32 i = 0; i < 11; i++)
	{
		BlueHealth.Add(600.0f + 30.0 * i);
	}

	for (int32 i = 0; i < 11; i++)
	{
		if (i < 6)
		{
			BlueAttack.Add(30.0f + 2.0f * i);
		}
		else
		{
			BlueAttack.Add(40.0f + 1.0f * (i - 5));
		}
	}

	for (int32 i = 0; i < 11; i++)
	{
		BlueAttackSpeed.Add(5.5f + 0.1f * i);
	}

	for (int32 i = 0; i < 11; i++)
	{
		BlueSkillValue.Add(17.0f + 1.0f * i);
	}

	for (int32 i = 0; i < 11; i++)
	{
		RedHealth.Add(850.0f + 50.0 * i);
	}
	
	for (int32 i = 0; i < 11; i++)
	{
		if (i < 6)
		{
			RedAttack.Add(25.0f + 2.0f * i);
		}
		else
		{
			RedAttack.Add(35.0f + 1.0f * (i - 5));
		}
	}

	for (int32 i = 0; i < 11; i++)
	{
		RedAttackSpeed.Add(5.1f + 0.1f * i);
	}

	for (int32 i = 0; i < 11; i++)
	{
		RedSkillValue.Add(35.0f + 2.0f * i);
	}

	for (int32 i = 0; i < 11; i++)
	{
		GreenHealth.Add(700.0f + 40.0 * i);
	}

	for (int32 i = 0; i < 11; i++)
	{
		if (i < 6)
		{
			GreenAttack.Add(27.0f + 2.0f * i);
		}
		else
		{
			GreenAttack.Add(37.0f + 1.0f * (i - 5));
		}
	}

	for (int32 i = 0; i < 11; i++)
	{
		GreenAttackSpeed.Add(5.2f + 0.1f * i);
	}

	for (int32 i = 0; i < 11; i++)
	{
		GreenSkillValue.Add(6.0 + 0.3f * i);
	}

	for (int32 i = 0; i < 11; i++)
	{
		MaximumStamina.Add(100.0f + 5.0f * i);
	}

	for (int32 i = 0; i < 11; i++)
	{
		if (i < 6)
		{
			GasCanDropRate.Add(0.0f + 2.0f * i);
		}
		else
		{
			GasCanDropRate.Add(10.0f + (i - 5));
		}
	}

	for (int32 i = 0; i < 11; i++)
	{
		RefinerCapacity.Add(20.0f + 3.0f * i);
	}

	for (int32 i = 0; i < 11; i++)
	{
		RefinerSpeed.Add(180.0f - i * 6.0f);
	}
}

float USkillSystem::GetData(const int32 SubjectIndex, const int32 InLevel)
{
	switch (SubjectIndex)
	{
	case 0:
		return BlueHealth[InLevel];
	case 1:
		return BlueAttack[InLevel];
	case 2:
		return BlueAttackSpeed[InLevel];
	case 3:
		return BlueSkillValue[InLevel];
	case 4:
		return RedHealth[InLevel];
	case 5:
		return RedAttack[InLevel];
	case 6:
		return RedAttackSpeed[InLevel];
	case 7:
		return RedSkillValue[InLevel];
	case 8:
		return GreenHealth[InLevel];
	case 9:
		return GreenAttack[InLevel];
	case 10:
		return GreenAttackSpeed[InLevel];
	case 11:
		return GreenSkillValue[InLevel];
	case 12:
		return MaximumStamina[InLevel];
	case 13:
		return GasCanDropRate[InLevel];
	case 14:
		return RefinerCapacity[InLevel];
	case 15:
		return RefinerSpeed[InLevel];
	default:
		UE_LOG(LogTemp, Warning, TEXT("Get Data Error"));
		return 0.0f;
	}
}

int32 USkillSystem::GetMaxLevel(const int32 SubjectIndex)
{
	switch (SubjectIndex)
	{
	case 0:
		return BlueHealth.Num() - 1;
	case 1:
		return BlueAttack.Num() - 1;
	case 2:
		return BlueAttackSpeed.Num() - 1;
	case 3:
		return BlueSkillValue.Num() - 1;
	case 4:
		return RedHealth.Num() - 1;
	case 5:
		return RedAttack.Num() - 1;
	case 6:
		return RedAttackSpeed.Num() - 1;
	case 7:
		return RedSkillValue.Num() - 1;
	case 8:
		return GreenHealth.Num() - 1;
	case 9:
		return GreenAttack.Num() - 1;
	case 10:
		return GreenAttackSpeed.Num() - 1;
	case 11:
		return GreenSkillValue.Num() - 1;
	case 12:
		return MaximumStamina.Num() - 1;
	case 13:
		return GasCanDropRate.Num() - 1;
	case 14:
		return RefinerCapacity.Num() - 1;
	case 15:
		return RefinerSpeed.Num() - 1;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Get Max Level Error"));
		return -1;
	}
}
