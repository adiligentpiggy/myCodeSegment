public class Unicode {    
    public static void main(String[] args) {
        String str = "张";
        System.out.println(Integer.toHexString(str.codePointAt(0)).toUpperCase());
    }
}