use std::collections::HashMap;

fn entry(table: &mut HashMap<i32, i32>, x: i32) -> i32 {
    if let Some(&v) = table.get(&x) {
        return v;
    }

    let a = entry(table, x - 1);
    let b = entry(table, a);
    let val = 1 + entry(table, x - b);

    table.insert(x, val);
    val
}
