package ru.ifmo.ctddev.trusienko.walk;

import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;

public class RecursiveWalk {
    private static String outputPath;
    private static BufferedWriter writer;

    private static int hashCount(InputStream reader) {
        int c, hash = 0x811c9dc5, p = 0x01000193;
        byte[] b = new byte[1024];
        try {
            while((c = reader.read(b)) >= 0) {
                for(int i = 0; i < c; i++) {
                    hash = (hash * p) ^ (b[i] & 0xff);
                }
            }
            return hash;
        } catch (IOException e) {
            return 0;
        }
    }

    private static Path getPath(String stringPath) {
        try{
            return Paths.get(stringPath);
        } catch (InvalidPathException e) {
            return null;
        }
    }

    private static void writeData(int hash, String path) {
        try {
            writer.write(String.format("%08x", hash) + " " + path);
            writer.newLine();
        } catch (IOException e) {
            System.err.println(outputPath + " - this file turned out unwritable");
            System.exit(0);
        }
    }

    private static void rec(Path inputPath) {
        if (Files.exists(inputPath)) {
            if (Files.isDirectory(inputPath)) {
                try {
                    DirectoryStream<Path> paths = Files.newDirectoryStream(inputPath);
                    for (java.nio.file.Path path : paths) {
                        rec(path);
                    }
                } catch (IOException e) {
                    writeData(0, inputPath.toString());
                }
            } else {
                try (InputStream reader = Files.newInputStream(inputPath)) {
                    writeData(hashCount(reader), inputPath.toString());
                } catch (IOException e) {
                    writeData(0, inputPath.toString());
                }
            }
        } else {
            writeData(0, inputPath.toString());
        }
    }

    public static void main(String args[]) {
        if(args == null || args.length < 2 || args[0] == null || args[1] == null) {
            System.err.println("Incorrect paths to input and output files");
            return;
        }
        outputPath = args[1];
        try (BufferedReader sourceData = Files.newBufferedReader(Paths.get(args[0]), StandardCharsets.UTF_8)) {
            try (BufferedWriter writerBad = Files.newBufferedWriter(Paths.get(args[1]), StandardCharsets.UTF_8)) {
                writer = writerBad;
                while (true) {
                    String stringPath = sourceData.readLine();
                    if (stringPath == null) {
                        break;
                    }
                    Path path = getPath(stringPath);
                    if(path == null) {
                        writeData(0, stringPath);
                        continue;
                    }
                    rec(path);
                }
            } catch (InvalidPathException e) {
                System.err.println(args[1] + " - incorrect path to output file");
            } catch (IOException e) {
                System.err.println(args[1] + " - output file is unwritable");
            } catch (SecurityException e) {
                System.err.println(args[1] + " - not enough rights to write");
            }
        } catch (InvalidPathException  e) {
            System.err.println(args[0] + " - incorrect path to input file");
        } catch (IOException e) {
            System.err.println(args[0] + " - input file is unreadable");
        } catch (SecurityException e) {
            System.err.println(args[0] + " - not enough rights to read");
        }
    }
}
