#include "Policy.h"

Policy::Policy()
{
}

Policy::~Policy()
{
}

bool Policy::set_policy(int policy_id)
{

	std::cout << "set_policy " << policy_id << std::endl;
	bool duplication = false;
 	switch (policy_id)
	{
	case 0:
		if (hearty_meal) duplication = true;
		hearty_meal = true;
		break;
	case 1:
		if (soup) duplication = true;
		soup = true;
		break;
	case 2:
		if (alcohol) duplication = true;
		alcohol = true;
		break;
	case 3:
		if (life_sustaining_treatment) duplication = true;
		life_sustaining_treatment = true;
		break;
	case 4:
		if (extreme_drug_prescription) duplication = true;
		extreme_drug_prescription = true;
		break;
	case 5:
		if (overcrowd) duplication = true;
		overcrowd = true;
		break;
	case 6:
		if (additional_serving_for_patient) duplication = true;
		additional_serving_for_patient = true;
		break;
	case 7:
		if (drug_treatment) duplication = true;
		drug_treatment = true;
		break;
	case 8:
		if (nursing_home) duplication = true;
		nursing_home = true;
		break;
	case 9:
		if (prosthetic_limb) duplication = true;
		prosthetic_limb = true;
		break;
	case 10:
		if (police) duplication = true;
		funeral = true;
		break;
	case 11:
		if (memorial) duplication = true;
		body_storage = true;
		break;
	case 12:
		if (body_storage) duplication = true;
		organ_transplant = true;
		break;
	case 13:
		if (cannibal) duplication = true;
		cannibal = true;
		break;
	case 14:
		if (memorial) duplication = true;
		memorial = true;
		break;
	case 15:
		if (police) duplication = true;
		police = true;
		break;
	case 16:
		if (guard_post) duplication = true;
		guard_post = true;
		break;
	case 17:
		if (prison) duplication = true;
		prison = true;
		break;
	case 18:
		if (martial_law) duplication = true;
		martial_law = true;
		break;
	case 19:
		if (dictator) duplication = true;
		dictator = true;
		break;
	case 20:
		if (emergency_shift_work) duplication = true;
		emergency_shift_work = true;
		break;
	case 21:
		if (overtime) duplication = true;
		overtime = true;
		break;
	case 22:
		if (censor) duplication = true;
		censor = true;
		break;
	case 23:
		if (forced_labor) duplication = true;
		forced_labor = true;
		break;
	case 24:
		if (child_labor) duplication = true;
		child_labor = true;
		break;
	case 25:
		if (nursery_school) duplication = true;
		nursery_school = true;
		break;
	case 26:
		if (student_soldier) duplication = true;
		student_soldier = true;
		break;
	case 27:
		if (labor_support) duplication = true;
		labor_support = true;
		break;
	default:
		break;
	}
	return duplication;

	// 0: 든든한 식사, 1:스프밥, 2: 밀주
// 3: 연명치료, 4: 극약처방, 5: 과잉수용, 6: 환자추가배식, 7: 마약치료 8: 요양원, 9: 의수족
// 10: 장례식, 11: 시체저장소, 12: 장기이식, 13: 식인, 14: 추모비
// 15: 자경단, 16: 경비초소, 17: 교도소, 18: 계엄령, 19: 독재자
// 20: 비상교대근무, 21: 연장근무, 22: 감찰관, 23: 강제노동
// 24: 아동노동, 25: 보육원, 26: 학도병징집, 27: 작업지원
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
