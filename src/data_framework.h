#pragma once
typedef struct Activity {
	bool enabled;
	uint8_t number;
	uint8_t data_type;
	int data_amount[2];
	char name[10];
	uint8_t length;
	char other[28];
	int goal;
	int accel_service;
} Activity;

typedef struct ActivityData { 
	Activity activities[9];
} ActivityData;

typedef struct ActivityDataPersist { 
	Activity activities[3];
} ActivityDataPersist;

void activity_copy(int number, Activity ACTV);
uint8_t get_amountof_activities();
void set_enabled(int activity, bool exists);
Activity get_activity(int activity);
void persist_load();
void persist_save();