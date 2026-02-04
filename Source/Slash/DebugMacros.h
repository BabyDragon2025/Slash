#pragma once

#include "DrawDebugHelpers.h" 

#define DRAW_SPHERE(Location) if(GetWorld()) DrawDebugSphere(GetWorld(),Location,25.f,12,FColor::Red,true);
#define DRAW_SPHERE_COLOR(Location, Color) DrawDebugSphere(GetWorld(),Location,8.f,12,Color,false,5.f);//画一个持续5秒的球体
#define DRAW_SPHERE_SingleFrame(Location) if(GetWorld()) DrawDebugSphere(GetWorld(),Location,25.f,12,FColor::Red,false,-1.f);
//单帧点的调试球                                                                        持续有限时间，-1.f为持续一帧。
#define DRAW_LINE(StartLocation,EndLocation) if(GetWorld()) DrawDebugLine(GetWorld(), StartLocation,EndLocation, FColor::Red, true, -1.f, 0, 1.f);
#define DRAW_LINE_SingleFrame(StartLocation,EndLocation) if(GetWorld()) DrawDebugLine(GetWorld(), StartLocation,EndLocation, FColor::Red, false, -1.f, 0, 1.f);
//单帧点的调试线，持续时间选择false
#define DRAW_POINT(Location) if(GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, true);
#define DRAW_POINT_SingleFrame(Location) if(GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, false,-1.f);
// 单帧点的调试点

//反斜杠可以把宏延续到下一行,这也是定义多行宏的方法。这种方法花括号里不可以加注释，一行只可以一个"\"。一定要注意："\"后面不可以有空格，会被识别成\\n
#define DRAW_VECTOR(StartLocation,EndLocation) if (GetWorld()) \
	{\
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f);\
		DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Red, true); \
	}
#define DRAW_VECTOR_SingleFrame(StartLocation,EndLocation) if (GetWorld()) \
	{\
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 1.f);\
		DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Red, false,-1.f); \
	}
