import java.util.HashMap;

class Main {
    private static int entry(int x) {
        final HashMap<Integer, Integer> table = new HashMap<Integer, Integer>();
        table.put(1, 1);

        if(table.get(x) != null) {
            return table.get(x);
        }

        int y = 1 + entry(x - entry(entry(x - 1)));
        table.put(x, y);
        return y;
    }
}
