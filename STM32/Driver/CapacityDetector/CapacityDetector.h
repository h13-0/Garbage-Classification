#ifndef _CAPACITYDETECTOR_H_
#define _CAPACITYDETECTOR_H_

void CapacityDetectorInit(void);

inline void CapacityDetector_Handler(void);

void CapacityDetector_getValue(double *rec, double *kit, double *oth, double *harm);

#endif
