export type Student = { id: number; name: string };

export type DailyArrangement = { title: string; list: number[] };

export type DailyLessons = string[];

export type LessonsTime = string[];

export type Notice = { title: string; text: string; date: "FOREVER" | string };

export type Event = { title: string; date: string; important: boolean };

export type Info = {
  students: Student[];
  arrangements: DailyArrangement[];
  lessons: DailyLessons;
  lessonsTm: LessonsTime;
  notices: Notice[];
  events: Event[];
};
