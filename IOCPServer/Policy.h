#pragma once
class Policy
{
public:
	Policy();
	~Policy();
	
	
	bool police = false;							//자경단
	bool guard_post = false;						//경비 초소
	bool prison = false;							//교도소
	bool martial_law = false;						//계엄령
	bool dictator = false;							//독재자
	
	
	bool funeral = false;							//장례식
	bool memorial = false;							//추모비
	bool body_storage = false;						//시체 저장소
	bool organ_transplant = false;					//장기이식
	bool cannibal = false;							//식인
	
	
	bool emergency_shift_work = false;				//비상 교대 근무
	bool overtime = false;							//연장 근무
	bool censor = false;							//감찰관
	bool forced_labor = false;						//강제 노동
	
	
	bool child_labor = false;						//아동 노동
	bool nursery_school = false;					//보육원
	bool labor_support = false;						//작업 지원
	bool student_soldier = false;					//학도병 징집
	
	
	bool hearty_meal = false;						//든든한 식사
	bool soup = false;								//스프 밥
	bool alcohol = false;							//밀주
	char get_meal_resource_consume();
	char get_meal_satiety();
	char get_meal_alcoholic();
	
	
	bool life_sustaining_treatment = false;			//연명치료
	bool extreme_drug_prescription = false;			//극약처방
	bool overcrowd = false;							//과잉수용
	bool additional_serving_for_patient = false;	//환자 추가 배식
	bool drug_treatment = false;					//마약 치료
	bool nursing_home = false;						//요양원
	bool prosthetic_limb = false;					//의수족
};
