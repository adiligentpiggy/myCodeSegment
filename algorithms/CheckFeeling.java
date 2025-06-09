
import java.io.InputStream;
import java.util.Scanner;
import java.io.IOException;

public class CheckFeeling {

    public static void checkFeelings(String teString) {
	    // write your code here
        int badCount = 0;
        int goodCount = 0;
        String feeling = ":-";
        String goodFeeling = ":-)";
        String badFeeling = ":-(";
        int start  = teString.indexOf(feeling);
        int step  = 3;
        while (start != -1 && start < teString.length() - 2) { 
            if (teString.substring(start, start + 3).equals(goodFeeling)) {
                goodCount++;
                step  = 3;
            } else if (teString.substring(start, start + 3).equals(badFeeling)) {
                badCount++;
                step  = 3;
            } else {
                step  = 2;
            }

            start  = teString.indexOf(feeling, start + step);
        }

        if (goodCount == 0 && badCount == 0) {
            System.out.println("none");
        } else if (goodCount > badCount) {
            System.out.println("happy");
        } else if (goodCount < badCount) {
            System.out.println("sad");
        } else {
            System.out.println("Just so so");
        }
    }
    public static void main(String[] args) {
	    // write your code here

        Scanner scanner = new Scanner(System.in);


        while (scanner.hasNextLine()) {
            String text = scanner.nextLine();
            checkFeelings(text);
        }

        scanner.close();
    }
}
