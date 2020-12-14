package Dictionary;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class DATrie {
    private final int ARRAY_SIZE = 655350;  //size of array
    private final int BASE_ROOT = 1;        //the state of root of base[]
    private final int BASE_NULL = 0;        //base empty state
    private final int CHECK_ROOT = -1;      //the state of root of check[]
    private final int CHECK_NULL = -2;      //check empty state
    private TrieNode base[];
    private int check[];


    /**
     *  @Description: DATrie node
     */
    public class TrieNode {
      private int transferRatio; // transfer cardinality
      private boolean isLeaf = false; // leaf node or not
      private Character label = null; // The node label is the inserted character itself
      private int value = -1; // When the node is a leaf node, the index of the corresponding input in the associated dictionary table

      public int getTransferRatio() {
        return transferRatio;
      }

      public void setTransferRatio(int transferRatio) {
        this.transferRatio = transferRatio;
      }

      public boolean isLeaf() {
        return isLeaf;
      }

      public void setLeaf(boolean leaf) {
        isLeaf = leaf;
      }

      public Character getLabel() {
        return label;
      }

      public void setLabel(Character label) {
        this.label = label;
      }

      public int getValue() {
        return value;
      }

      public void setValue(int value) {
        this.value = value;
      }
    }

    
    /**
     *  @Description: build the DATrie
     */
    public void build(List<String> words) {
      init();

      boolean shut = false;
      for (int idx = 0; idx < words.size(); idx++) {
        int startState = 0;
        char chars[] = words.get(idx).toCharArray();

        if (shut == false) {
          TrieNode node = insert(startState, getCode(chars[0]), (chars.length == 1), idx);
          node.setLabel(chars[0]);
        } else {
          for (int j = 1; j < chars.length; j++) {
            startState = transfer(startState, getCode(chars[j - 1]));
            TrieNode node = insert(startState, getCode(chars[j]), (chars.length == j + 1), idx);
            node.setLabel(chars[j]);
          }
        }

        if (idx == words.size() - 1 && shut == false) {
          idx = -1; // Because there was a process of adding at the beginning
          shut = true;
        }

      }
    }

    
    /**
     *  @Description: Query match (forward match)
     */
    public List<Integer> match(String keyWord) {
        List<Integer> result = new ArrayList<Integer>();
        int startState, endState;

        char chars[] = keyWord.toCharArray();
        for (int i = 0; i < chars.length; i++) {
            startState = 0;
            for (int j = i; j < chars.length; j++) {
                endState = transfer(startState, getCode(chars[j]));
                if (base[endState].getTransferRatio() != BASE_NULL && check[endState] == startState) { // The node is on the trie
                    if (base[endState].isLeaf()) {
                        if (!result.contains(base[endState].getValue())) {
                            result.add(base[endState].getValue());
                        }
                    }
                    startState = endState;
                } else {
                    break;
                }
            }
        }

        return result;
    }


    
    /**
     *  @Description: print the DATrie
     */
    public void printTrie() {
        System.out.println();
        System.out.printf("%5s", "idx");
        for (int i = 0; i < ARRAY_SIZE; i++) {
            if (base[i].getTransferRatio() != BASE_NULL) {
                System.out.printf("%7d\t", i);
            }
        }
        System.out.println();
        System.out.printf("%5s", "char");
        for (int i = 0; i < ARRAY_SIZE; i++) {
            if (base[i].getTransferRatio() != BASE_NULL) {
                System.out.printf("%7c\t", base[i].getLabel());
            }
        }
        System.out.println();
        System.out.printf("%5s", "base");
        for (int i = 0; i < ARRAY_SIZE; i++) {
            if (base[i].getTransferRatio() != BASE_NULL) {
                System.out.printf("%7d\t", base[i].getTransferRatio());
            }
        }
        System.out.println();
        System.out.printf("%5s", "check");
        for (int i = 0; i < ARRAY_SIZE; i++) {
            if (base[i].getTransferRatio() != BASE_NULL) {
                System.out.printf("%7d\t", check[i]);
            }
        }
        System.out.println();
        System.out.printf("%5s", "leaf");
        for (int i = 0; i < ARRAY_SIZE; i++) {
            if (base[i].getTransferRatio() != BASE_NULL) {
                System.out.printf("%7s\t", base[i].isLeaf() ? "是" : "否");
            }
        }
        System.out.println();
        System.out.printf("%5s", "idx");
        for (int i = 0; i < ARRAY_SIZE; i++) {
            if (base[i].getTransferRatio() != BASE_NULL) {
                System.out.printf("%7d\t", base[i].getValue());
            }
        }
        System.out.println();
    }



    /**
     *  @Description: Insert a new node according to the starting state and transfer technique and return the inserted node
     *  @param startState start state
     *  @param offset  off set of the state
     *  @param isLeaf  is a leaf node or not
     *  @param idx The index number of the current node in the dictionary
     */
    private TrieNode insert(int startState, int offset, boolean isLeaf, int idx) {
        int endState = transfer(startState, offset); //transfer state

        if (base[endState].getTransferRatio() != BASE_NULL && check[endState] != startState) { // not empty
            do {
                endState += 1;
            } while (base[endState].getTransferRatio() != BASE_NULL);

            base[startState].setTransferRatio(endState - offset); // change the transfer cardinality of the parent node

        }

        if (isLeaf) {
            base[endState].setTransferRatio(Math.abs(base[startState].getTransferRatio())*-1); // The leaf node transfer cardinality is identified as the opposite number of the parent node transfer cardinality
            base[endState].setLeaf(true);
            base[endState].setValue(idx); // if is a leaf node, the index of the word in the dictionary should be recorded
        } else {
            if (base[endState].getTransferRatio() == BASE_NULL) { // no node passes
                base[endState].setTransferRatio(Math.abs(base[startState].getTransferRatio())); // the transfer cardinality of the non-leaf node should always be positive
            }
        }
        check[endState] = startState;// check records the parent state of the current state

        return base[endState];
    }



    /**
     *  @Description: Return the end state according to start state and transfer cardinality
     */
    private int transfer(int startState, int offset) {
        return Math.abs(base[startState].getTransferRatio())+offset; // offset of state
    }


    /**
     *  @Description: Get the index of base[]
     */
    private int getCode(char c) {
        return (int)c;// should be greater than 0
    }



    /**
     *  @Description: Initialize DATrie（Initialize both base[], and check[]）
     */
    private void init() {
        base = new TrieNode[ARRAY_SIZE];
        check = new int[ARRAY_SIZE];

        for (int i = 0; i < ARRAY_SIZE; i++) {
            TrieNode node = new TrieNode();
            node.setTransferRatio(BASE_NULL);
            base[i] = node;
            check[i] = CHECK_NULL;
        }

        TrieNode root = new TrieNode();
        root.setTransferRatio(BASE_ROOT);
        base[0] = root;
        check[0] = CHECK_ROOT;
    }


    public static void main(String[] args) {

        List<String> words = new ArrayList<String>();
        words.add("清华");
        words.add("清华大学");
        words.add("清新");
        words.add("中华");
        words.add("中华人民");
        words.add("华人");
        words.add("学生");
        words.add("大学生");
        words.add("wo");
        words.add("shi");
        words.add("human");
        words.add("this");
        words.add("is");
        words.add("ragty");
        words.add("pump");
        words.add("it");
        words.add("up");
        words.add("中国");
        words.add("人名");
        words.add("中国人民");
        words.add("人民");
        words.add("java");
        words.add("java学习");



        // Build Trie
        DATrie daTrie = new DATrie();
        daTrie.build(words);
        daTrie.printTrie();

        String keyWord = "清华大学生都是华人";
        List<Integer> result = daTrie.match(keyWord);
        System.out.println();
        System.out.println("输入语句为："+keyWord);

        // Print the match result
        System.out.println();
        System.out.printf("Match: {");
        for (int i = 0; i < result.size(); i++) {
            if (i == 0) {
                System.out.printf("%s", words.get(result.get(i)));
            } else {
                System.out.printf(", %s", words.get(result.get(i)));
            }
        }
        System.out.printf("}");
        System.out.println();
    }

}