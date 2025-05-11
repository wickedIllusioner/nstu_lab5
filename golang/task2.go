package main

import (
	"fmt"
	"math/rand"
	"sort"
	"strings"
	"time"
)

type Client struct {
	ticket   string
	duration int
}

// Вывод талонов в одну строку
func printClients(clients []Client) string {
	var builder strings.Builder
	for _, client := range clients {
		builder.WriteString(client.ticket)
		builder.WriteString(" ")
	}
	return builder.String()
}

func Input() string {
	return "<<< "
}

func Output() string {
	return ">>> "
}

func TotalDuration(clients []Client) string {
	sum := 0
	for _, client := range clients {
		sum += client.duration
	}
	return fmt.Sprintf(" (%d мин.): ", sum)
}

// Генерация талона
func GenerateTicket() string {
	rand.Seed(time.Now().UnixNano())
	num := rand.Intn(1000) + 1
	return fmt.Sprintf("T%03d", num)
}

// Запись в общую очередь по талону
func Enqueue(queue *[]Client, client *Client) {
	client.ticket = GenerateTicket()
	*queue = append(*queue, *client)

	fmt.Println(Output() + client.ticket)
}

// Распределение посетителей по окнам
func Distribute(windows *map[int][]Client, clients []Client, total int) {
	// Сортировка посетителей в порядке убывания времени
	sort.Slice(clients, func(i, j int) bool {
		return clients[i].duration > clients[j].duration
	})

	// Массив для хранения текущей суммы времени для каждого окна
	windowTimes := make([]int, total)
	tempWindows := make(map[int][]Client)
	for i := 1; i <= total; i++ {
		tempWindows[i] = []Client{}
	}

	// Распределяем клиентов
	for _, client := range clients {
		// Находим окно с минимальным текущим временем
		minWindow := 1
		for i := 2; i <= total; i++ {
			if windowTimes[i-1] < windowTimes[minWindow-1] {
				minWindow = i
			}
		}
		// Добавляем клиента в это окно
		tempWindows[minWindow] = append(tempWindows[minWindow], client)
		windowTimes[minWindow-1] += client.duration
	}

	// Переносим распределение в основную структуру
	*windows = tempWindows

	for windowNum, windowClients := range *windows {
		fmt.Printf("%sОкно %d%s%s\n",
			Output(),
			windowNum,
			TotalDuration(windowClients),
			printClients(windowClients))
	}
}

func main() {
	queue := make(map[int][]Client) // Окна регистратуры
	var clients []Client            // Общая очередь из посетителей
	var totalWindows int

	fmt.Println(Output() + "Введите количество окон")
	fmt.Print(Input())
	_, err := fmt.Scan(&totalWindows)
	if err != nil || totalWindows <= 0 {
		fmt.Println("Некорретное количество окон. Попробуйте снова")
		return
	}
	for i := 1; i <= totalWindows; i++ {
		queue[i] = []Client{}
	}

	var operation string
	for {
		fmt.Print(Input())
		_, err := fmt.Scan(&operation)
		if err != nil {
			continue
		}

		if operation == "ENQUEUE" {
			var duration int
			_, err := fmt.Scan(&duration)
			if err != nil {
				continue
			}
			client := Client{duration: duration}
			Enqueue(&clients, &client)
		} else if operation == "DISTRIBUTE" {
			Distribute(&queue, clients, totalWindows)
			break
		} else {
			fmt.Println(Output() + "Неизвестная команда")
		}
	}
}

