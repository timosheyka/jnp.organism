#ifndef ORGANISM_H
#define ORGANISM_H

#include <concepts>
#include <optional>
#include <stdexcept>
#include <tuple>

namespace {

    template <std::equality_comparable species_t, bool can_eat_meat,
            bool can_eat_plants>
    class Organism {
    private:
        species_t const species;
        uint64_t vitality;

    public:
        constexpr Organism(species_t const& species, uint64_t vitality)
                : species(species), vitality(vitality) {}

        constexpr Organism<species_t, can_eat_meat, can_eat_plants> clone() const {
            return Organism<species_t, can_eat_meat, can_eat_plants>(species,
                    vitality);
        }

        constexpr Organism<species_t, can_eat_meat, can_eat_plants> clone(
                uint64_t newVitality) const {
            return Organism<species_t, can_eat_meat, can_eat_plants>(species,
                                                                     newVitality);
        }

        constexpr bool is_dead() const { return vitality == 0; }

        constexpr uint64_t get_vitality() const { return vitality; }

        constexpr species_t const& get_species() const { return species; }
    };

    static constexpr uint64_t safeAdd(const uint64_t a, const uint64_t b) {
        if (a + b < std::max(a, b)) {
            throw std::out_of_range("");
        }
        return a + b;
    }

    template <std::equality_comparable T>
    using Carnivore = Organism<T, true, false>;

    template <std::equality_comparable T>
    using Omnivore = Organism<T, true, true>;

    template <std::equality_comparable T>
    using Herbivore = Organism<T, false, true>;

    template <std::equality_comparable T>
    using Plant = Organism<T, false, false>;

    template <bool sp1_eats_m, bool sp1_eats_p, bool sp2_eats_m, bool sp2_eats_p>
    concept areBothPlants = (!sp1_eats_m && !sp1_eats_p && !sp2_eats_m &&
                             !sp2_eats_p);

    template <typename species_t, bool sp1_eats_m, bool sp1_eats_p, bool sp2_eats_m,
            bool sp2_eats_p>
    requires(!areBothPlants<sp1_eats_m, sp1_eats_p, sp2_eats_m, sp2_eats_p>)
    constexpr std::tuple<
            Organism<species_t, sp1_eats_m, sp1_eats_p>,
            Organism<species_t, sp2_eats_m, sp2_eats_p>,
            std::optional<Organism<
                    species_t, sp1_eats_m,
                    sp1_eats_p>>> encounter(Organism<species_t, sp1_eats_m, sp1_eats_p>
    organism1,
    Organism<species_t, sp2_eats_m, sp2_eats_p>
            organism2) {
    if (!organism1.get_vitality() || !organism2.get_vitality())
    return std::make_tuple(organism1.clone(), organism2.clone(),
            std::nullopt);

    if (std::same_as<decltype(organism1), decltype(organism2)> &&
            organism1.get_species() == organism2.get_species()) {
    return std::make_tuple(
            organism1.clone(), organism2.clone(),
            organism1.clone(
            safeAdd(organism1.get_vitality(), organism2.get_vitality()) /
    2));
}

constexpr bool isOrganism1Plant = !sp1_eats_p && !sp1_eats_m,
        isOrganism2Plant = !sp2_eats_p && !sp2_eats_m;

if constexpr (!((isOrganism1Plant && sp2_eats_p) ||
(isOrganism2Plant && sp1_eats_p) ||
(!isOrganism1Plant && !isOrganism2Plant &&
(sp1_eats_m || sp2_eats_m)))) {
return std::make_tuple(organism1.clone(), organism2.clone(),
        std::nullopt);
}

if constexpr (!isOrganism1Plant && !isOrganism2Plant && sp1_eats_m &&
sp2_eats_m) {
if (organism1.get_vitality() == organism2.get_vitality())
return std::make_tuple(organism1.clone(0), organism2.clone(0),
std::nullopt);

if (organism1.get_vitality() < organism2.get_vitality())
return std::make_tuple(
        organism1.clone(0),
organism2.clone(safeAdd(organism2.get_vitality(),
                        organism1.get_vitality() / 2)),
std::nullopt);

if (organism1.get_vitality() > organism2.get_vitality())
return std::make_tuple(
        organism1.clone(safeAdd(organism1.get_vitality(),
                                organism2.get_vitality() / 2)),
organism2.clone(0), std::nullopt);
}

if constexpr (isOrganism1Plant && sp2_eats_p)
return std::make_tuple(
        organism1.clone(0),
organism2.clone(
        safeAdd(organism2.get_vitality(), organism1.get_vitality())),
std::nullopt);

if constexpr (isOrganism2Plant && sp1_eats_p)
return std::make_tuple(
        organism1.clone(
        safeAdd(organism1.get_vitality(), organism2.get_vitality())),
organism2.clone(0), std::nullopt);

if constexpr (sp1_eats_m)
return organism1.get_vitality() <= organism2.get_vitality()
? std::make_tuple(organism1.clone(), organism2.clone(),
        std::nullopt)
: std::make_tuple(
        organism1.clone(safeAdd(organism1.get_vitality(),
                                organism2.get_vitality() / 2)),
organism2.clone(0), std::nullopt);

if constexpr (sp2_eats_m)
return organism2.get_vitality() <= organism1.get_vitality()
? std::make_tuple(organism1.clone(), organism2.clone(),
        std::nullopt)
: std::make_tuple(
        organism1.clone(0),
organism2.clone(safeAdd(organism2.get_vitality(),
                        organism1.get_vitality() / 2)),
std::nullopt);
}

template <typename species_t, bool sp1_eats_m, bool sp1_eats_p,
        typename... Args>
constexpr Organism<species_t, sp1_eats_m, sp1_eats_p> encounter_series(
        Organism<species_t, sp1_eats_m, sp1_eats_p> organism1, Args... args) {
auto encounterResult = organism1.clone();

return encounterResult;
}

template <typename species_t, bool sp1_eats_m, bool sp1_eats_p, bool sp2_eats_m,
        bool sp2_eats_p, typename... Args>
constexpr Organism<species_t, sp1_eats_m, sp1_eats_p> encounter_series(
        Organism<species_t, sp1_eats_m, sp1_eats_p> organism1,
Organism<species_t, sp2_eats_m, sp2_eats_p> organism2, Args... args) {
auto encounterResult = encounter(organism1, organism2);
auto result = std::get<0>(encounterResult);

return encounter_series(result, args...);
}

}  // namespace

#endif  // ORGANISM_H
