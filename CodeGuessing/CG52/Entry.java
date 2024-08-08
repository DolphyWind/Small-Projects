import java.lang.StringBuilder;

class Entry {
    public static String entry(int n) {
        StringBuilder sb = new StringBuilder();
        
        int len = (int)Math.pow(2, n);
        for(int i = 0; i < len; i++) {
            int popcnt = Integer.bitCount(i);
            if((popcnt & 1) == 0) {
                sb.append('0');
            }
            else {
                sb.append('1');
            }
        }

        return sb.toString();
    }
}
