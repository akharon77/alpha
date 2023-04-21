# Цель работы
Это продолжение работы (после Мандельброта) по оптимизации програм с использованием SIMD команд

# Ход работы
1. Написал наивную реализацю
2. Написал оптимизированную реализацию
3. Измерил производительность оптимизировнной и наивной версии с учётом и без оптимизации компилятором. 
4. Воспользовался инструментом godbolt.org и сравнил фрагменты кода на Си и инструкции, которые получаются на выходе компилятор
5. Рассмотрел наивную реализацию, которая рассчитывает каждый пиксель отдельно
6. Рассмотрел векторизованную версию с использованием SIMD команд
7. Сделал вывод

# Сравнение выхода компилятора

```
```

## Наивная версия с `-O2`:
```
```

## Оптимизировнная версия с `-O2`:
```
```

# Результаты замеров
Аналогично задаче по построение множества Мандельброта, чтобы замерить именно производительность вычисления, я проводил вычисления картинки фиксированное количество раз, в моём случае 200, и с его учётом измерял количество кадров в секунду. Так я смог пренебречь затратами на отрисовку кадра по сравнению с её расчётом, искусственно сместив вес вычислений. В таблице приведены замеры FPS усредненные за 10 секунд. Программа работает очень быстро и замерять быстроменяющееся значение количество кадров в секунду не представляется удобным

Я привёл результаты замеров с флагами `-O0` и `-O2` для того, чтобы показать:
1. Компилятор пытается использовать **XMM** регистры для ускорения расчётов с плавающей точкой
2. Компилятор может оптимизировать код, написанный с использованием интринсиков, в отличие от кода с ассемблерными вставками

**Оптимизированная**:
| FPS                    | `-O0`  | `-O2`   |
|------------------------|--------|---------|
|                        | 406.01 | 2770.28 |
|                        | 412.32 | 2607.26 |
|                        | 396.34 | 2725.50 |
|                        | 396.34 | 2883.61 |
|                        | 396.34 | 2792.89 |
| Среднее                | 396.34 | 2755.91 |
| Стандартное отклонение | 7.96   | 101.12  |
|                        |        |         |

**Наивная**:
| FPS                    | `-O0` | `-O2` |
|------------------------|-------|-------|
|                        | 2.81  | 5.14  |
|                        | 2.92  | 5.20  |
|                        | 2.83  | 5.14  |
|                        | 2.77  | 5.19  |
|                        | 2.87  | 5.29  |
| Среднее                | 2.84  | 5.19  |
| Стандартное отклонение | 0.06  | 0.06  |

# Вывод
Таким образом, исходя из данных в таблице, я получил ускорение программы (сравнивая версии с оптимизациями компилятора) примерно в 12 раз

