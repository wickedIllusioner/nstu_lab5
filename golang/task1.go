package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

const MAX_CELL_CAPACITY = 10

// Структура для товара
type CellUnit struct {
	good   string
	amount int
}

// Вывод содержимого массива CellUnit
func printCellUnits(v []CellUnit) string {
	var builder strings.Builder
	for _, elem := range v {
		builder.WriteString(fmt.Sprintf("%s, %d; ", elem.good, elem.amount))
	}
	return builder.String()
}

// Вывод инструкции
func getInstructions() {
	fmt.Println("==================================")
	fmt.Println("1. ADD <товар> <кол-во> <ячейка> - добавить товар;")
	fmt.Println("2. REMOVE <товар> <кол-во> <ячейка> - удалить товар")
	fmt.Println("3. INFO - получить информацию о складе")
	fmt.Println("==================================")
	fmt.Print("Ввод: ")
}

// Количество товаров в ячейке
func cellSpace(cell []CellUnit) int {
	sum := 0
	for _, elem := range cell {
		sum += elem.amount
	}
	return sum
}

// Проверка на наличие ячейки на складе
func checkCell(cell string, db map[string][]CellUnit) bool {
	_, exists := db[cell]
	return exists
}

// Удаление товара из ячейки
func removeFromCell(db map[string][]CellUnit, good string, amount int, cell string) bool {
	total := 0
	for _, elem := range db[cell] {
		if elem.good == good {
			total += elem.amount
		}
	}
	if total < amount {
		return false
	}

	var newCell []CellUnit
	remains := amount
	for _, elem := range db[cell] {
		if elem.good == good {
			if elem.amount > remains {
				newCell = append(newCell, CellUnit{elem.good, elem.amount - remains})
				remains = 0
			} else {
				remains -= elem.amount
			}
		} else {
			newCell = append(newCell, elem)
		}
	}
	db[cell] = newCell
	return true
}

// Загруженность склада в процентах
func warehouseWorkload(db map[string][]CellUnit) (float64, float64, float64) {
	var areaTotal, areaA, areaB float64

	for cell, units := range db {
		space := float64(cellSpace(units))
		areaTotal += space
		if strings.HasPrefix(cell, "А") {
			areaA += space
		} else if strings.HasPrefix(cell, "Б") {
			areaB += space
		}
	}

	totalPercent := areaTotal / 7200 * 100
	aPercent := areaA / 3600 * 100
	bPercent := areaB / 3600 * 100

	return totalPercent, aPercent, bPercent
}

func main() {
	db := make(map[string][]CellUnit)
	zones := []string{"А", "Б"}

	// Создание ячеек хранения
	for i := 0; i < 2; i++ {
		for stell := 1; stell <= 4; stell++ {
			for vert := 1; vert <= 18; vert++ {
				for shelf := 1; shelf <= 5; shelf++ {
					cell := zones[i] + strconv.Itoa(stell) + strconv.Itoa(vert) + strconv.Itoa(shelf)
					db[cell] = []CellUnit{}
				}
			}
		}
	}

	// Основной функционал
	scanner := bufio.NewScanner(os.Stdin)

	for {
		getInstructions()
		scanner.Scan()
		input := scanner.Text()
		parts := strings.Fields(input)
		if len(parts) == 0 {
			continue
		}

		operation := parts[0]
		if operation == "EXIT" {
			break
		}

		switch operation {
		case "ADD":
			if len(parts) != 4 {
				fmt.Println("Неверное количество аргументов для ADD")
				continue
			}
			good := parts[1]
			amount, err := strconv.Atoi(parts[2])
			if err != nil {
				fmt.Println("Неверное количество товара")
				continue
			}
			cell := parts[3]

			if !checkCell(cell, db) {
				fmt.Println("Ячейки не существует. Повторите ввод снова")
				continue
			}
			if cellSpace(db[cell])+amount <= MAX_CELL_CAPACITY {
				db[cell] = append(db[cell], CellUnit{good, amount})
				fmt.Println("Успешно!")
			} else {
				fmt.Println("Невозможно выполнить операцию: количество товаров в ячейке превышает допустимый максимум")
				continue
			}

		case "REMOVE":
			if len(parts) != 4 {
				fmt.Println("Неверное количество аргументов для REMOVE")
				continue
			}
			good := parts[1]
			amount, err := strconv.Atoi(parts[2])
			if err != nil {
				fmt.Println("Неверное количество товара")
				continue
			}
			cell := parts[3]

			if !checkCell(cell, db) {
				fmt.Println("Ячейки не существует. Повторите ввод снова")
				continue
			}
			if removeFromCell(db, good, amount, cell) {
				fmt.Println("Удаление завершено!")
			} else {
				fmt.Println("Невозможно выполнить операцию: введенное число превышает количество товара в ячейке или товар отсутствует")
				continue
			}

		case "INFO":
			total, a, b := warehouseWorkload(db)
			fmt.Printf("Общая загруженность склада: %.2f%%\n", total)
			fmt.Printf("Загруженность зоны А: %.2f%%\n", a)
			fmt.Printf("Загруженность зоны Б: %.2f%%\n", b)
			for cell, units := range db {
				if cellSpace(units) != 0 {
					fmt.Printf("%s: %s\n", cell, printCellUnits(units))
				}
			}
			fmt.Print("Пустые ячейки: ")
			for cell, units := range db {
				if cellSpace(units) == 0 {
					fmt.Printf("%s ", cell)
				}
			}
			fmt.Println()

		default:
			fmt.Println("Неизвестная операция. Повторите ввод снова")
		}
	}
}

