#include "Policy.h"

Policy::Policy()
{
}

Policy::~Policy()
{
}

void Policy::set_policy(int policy_id)
{
	// 0: 든든한 식사, 1:스프밥, 2: 밀주
	// 3: 연명치료, 4: 극약처방, 5: 과잉수용, 6: 환자추가배식, 7: 마약치료 8: 요양원, 9: 의수족
	// 10: 장례식, 11: 시체저장소, 12: 장기이식, 13: 식인, 14: 추모비
	// 15: 자경단, 16: 경비초소, 17: 교도소, 18: 계엄령, 19: 독재자
	// 20: 비상교대근무, 21: 연장근무, 22: 감찰관, 23: 강제노동
	// 24: 아동노동, 25: 보육원, 26: 학도병징집, 27: 작업지원
	std::cout << "set_policy " << policy_id << std::endl;
 	switch (policy_id)
	{
	case 0:
		hearty_meal = true;
		break;
	case 1:
		soup = true;
		break;
	case 2:
		alcohol = true;
		break;
	case 3:
		life_sustaining_treatment = true;
		break;
	case 4:
		extreme_drug_prescription = true;
		break;
	case 5:
		overcrowd = true;
		break;
	case 6:
		additional_serving_for_patient = true;
		break;
	case 7:
		drug_treatment = true;
		break;
	case 8:
		nursing_home = true;
		break;
	case 9:
		prosthetic_limb = true;
		break;
	case 10:
		funeral = true;
		break;
	case 11:
		body_storage = true;
		break;
	case 12:
		organ_transplant = true;
		break;
	case 13:
		cannibal = true;
		break;
	case 14:
		memorial = true;
		break;
	case 15:
		police = true;
		break;
	case 16:
		guard_post = true;
		break;
	case 17:
		prison = true;
		break;
	case 18:
		martial_law = true;
		break;
	case 19:
		dictator = true;
		break;
	case 20:
		emergency_shift_work = true;
		break;
	case 21:
		overtime = true;
		break;
	case 22:
		censor = true;
		break;
	case 23:
		forced_labor = true;
		break;
	case 24:
		child_labor = true;
		break;
	case 25:
		nursery_school = true;
		break;
	case 26:
		student_soldier = true;
		break;
	case 27:
		labor_support = true;
		break;
	default:
		break;
	}
}

char Policy::get_meal_resource_consume()
{
	if (hearty_meal)
		return 2;
	else
		return 1;
}

char Policy::get_meal_satiety()
{
	if (hearty_meal)
		return 40;
	else if (soup)
		return 40;
	else
		return 20;
}

char Policy::get_meal_alcoholic()
{
	if (alcohol)
		return 10;
	else
		return 0;
}
