fn print_stuff(a:String) {
    println!("{0}", a);
}

fn array_stuff(a:&mut [u128]) {
    a[0] = 4;
    println!("{:?} {}", a, a.len());
}

fn main() {
    let b = &mut [1, 2, 3, 400];
    array_stuff(b);
    print_stuff("Hello World this is a rust program".to_string());
}