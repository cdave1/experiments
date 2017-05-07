package main

import "fmt"

// DontChangeMe should never change
const DontChangeMe string = "no changes allowed!"

func main() {
	var i int
	var i32 int32
	var u32 uint32
	var s string
	var f32 float32

	fmt.Println(i, i32, s, f32)

	i = 42
	overflowed := u32 - 1
	fmt.Println(i, overflowed)

	u64 := u32
	fmt.Println(u64)

	fmt.Println(DontChangeMe)
}
