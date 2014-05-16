#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include <vector>
#include <functional>

namespace smoga
{

    template<typename F, typename DATA>
    /**
     * @brief The Chromosome class Provide some basic datas and functions to manage a Chromosome in GAs.
     */
    class Chromosome
    {
    protected:
        std::vector < F > m_fitness;        ///> Fitness of the individual for the given problem
        std::vector< DATA > m_datas;        ///> Describe how the chromosome is represented to apply the GA
        static unsigned int m_nbGenes;      ///> Number of genes

    public:
        Chromosome();
        virtual ~Chromosome();

        /**
         * @brief mutate Method call to apply mutation on the chromosome.
         */
        virtual bool mutate() =0;

        /**
         * @brief computeFitness Compute the fitness of the chromosome.
         */
        virtual void computeFitness() =0;

        /**
         * @brief generateRandomChromosome Generate Random values for the chromosome.
         */
        virtual void generateRandomChromosome() =0;

        ////////////// Accessor/Setters //////////////
        /**
         * @brief getFitness Fitness of the chromosome.
         * @return The fitness.
         */
        inline std::vector < F > getFitness() const { return m_fitness; }

        /**
         * @brief getNbObjective Get the number of objective.
         * @return Number of Objective.
         */
        inline unsigned int getNbObjective() const { return m_fitness.size(); }

        /**
         * @brief getDatas Allow to get all genes of the chromosome.
         * @return Genes's vector of the chromosome.
         */
        inline std::vector< DATA > getDatas() const { return m_datas; }
        /**
         * @brief setDatas Set all genes of the chromosome to the parameter value.
         * @param datas Genes's vector of the chromosome
         */
        inline void setDatas(const std::vector< DATA > datas) { m_datas = datas; }

        /**
         * @brief datasToStr Convert the vector of DATA into a string.
         * @return String representing m_datas.
         */
        virtual std::string datasToStr();
        /**
         * @brief datasToChar Convert the vector of DATA into a string.
         * @return Chain of character representing m_datas.
         */
        virtual const char* datasToChar() { return datasToStr().c_str(); }

        /**
         * @brief setNbGenes Set the number of genes composing the chromosome.
         * @param nbGenes The number of genes wanted.
         */
        static void setNbGenes(const int nbGenes) { m_nbGenes = nbGenes; }
        /**
         * @brief getNbGenes Get the number of genes composing the chromosome.
         * @return The number of genes.
         */
        static int getNbGenes() { return m_nbGenes; }

        // Operator
        bool operator<(const Chromosome<F, DATA>& chromosome) const { return m_fitness < chromosome.m_fitness; }
        bool operator ==(const Chromosome<F, DATA>& chromosome) const { return m_fitness == chromosome.m_fitness && m_datas == chromosome.m_datas; }
    };

    template<typename F, typename DATA>
    unsigned int Chromosome<F, DATA>::m_nbGenes = 10;

    template<typename F, typename DATA>
    Chromosome<F, DATA>::Chromosome()
    {
    }

    template<typename F, typename DATA>
    Chromosome<F, DATA>::~Chromosome()
    {
    }

    template<typename F, typename DATA>
    std::string Chromosome<F, DATA>::datasToStr()
    {
        std::string datasStr = "";
        for (unsigned int i = 0 ; i < m_datas.size() ; i++)
            datasStr += m_datas[i] + '0';
        return datasStr;
    }

}

#endif // CHROMOSOME_H
