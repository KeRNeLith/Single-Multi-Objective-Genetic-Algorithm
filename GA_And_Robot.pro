TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS =   GeneticAlgorithms \
            GUIGeneticAlgorithms

GeneticAlgorithms.file = ./GA-NSGAII/GeneticAlgorithms.pro

GUIGeneticAlgorithms.file = ./GUIGeneticAlgorithms/GUIGeneticAlgorithms.pro
