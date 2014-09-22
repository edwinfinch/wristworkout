#pragma once
typedef struct Activity {
	bool enabled;
	uint8_t data_type;
	char name[10];
	char other[28];
	int goal;
	uint8_t used;
} Activity;

typedef struct ActivityData { 
	Activity activities[9];
} ActivityData;

typedef struct ActivityDataPersist { 
	Activity activities[3];
} ActivityDataPersist;

void activity_copy(int number, Activity *ACTV);
void activity_copy_alt(int number, Activity ACTV);
uint8_t get_amountof_activities();
void set_enabled(int activity, bool exists);
Activity get_activity(int activity);
void persist_load();
void persist_save();
void used_stack_push(int activity);
int last_used_ACTVs(int stack_num);