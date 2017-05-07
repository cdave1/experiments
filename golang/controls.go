// Control structures

package main

import "fmt"
import "math"

func main() {
	i := 0

	// While true...
	for {
		if i > 5 {
			break
		}
		fmt.Println(i)
		i++
	}

	i = 0
	for ; i < 5; i++ {
		fmt.Println(i)
	}

	for i = 5; i >= 0; i-- {
		fmt.Println(i)
	}

	for k := 0; k < 5; k++ {
		fmt.Println(k)
	}

	for x := 0.0; x < 1.0; x += 0.05 {
		a := 10 * (math.Cos(math.Pi*2.0*x)*0.5 + 0.5)
		for ; a >= 0.0; a -= 1.0 {
			fmt.Print("=")
		}
		fmt.Println()
	}
}
