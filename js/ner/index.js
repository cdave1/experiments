var mitie = require('mitie');
 
// Point it at a ner model .dat file 
// You can also find where to get these on the main MIT project repo 
 
var nerFile = '/usr/local/Cellar/mitie/0.4/share/MITIE-models/english/ner_model.dat';
//var nerFile = '/usr/local/Cellar/mitie/0.4/share/MITIE-models/english/total_word_feature_extractor.dat'
var neExtractor = new mitie.EntityExtractor(nerFile);
//var neExtractor = new mitie.FeatureExtractor(nerFile);// .EntityExtractor(nerFile);
 
// Feed in some data to the predict method 
 
//ar data = "Barack Obama visited New Orleans today for the anniversary of Hurricane Katrina."
var data = "zachariah photography studio" //james brown suede shoes"
var results = neExtractor.predict(data);

console.log(results);

/**
 * results => {
 *  tags:
 *    [ { tagNumber: 0,
 *        score: 1.0957487452879051,
 *        tagType: 'PERSON',
 *        tokens: 'Barack Obama' },
 *      { tagNumber: 1,
 *        score: 0.8764343236070008,
 *        tagType: 'LOCATION',
 *        tokens: 'New Orleans' } ] }
 */
 
var relFile = '/usr/local/Cellar/mitie/0.4/share/MITIE-models/english/binary_relations/rel_classifier_people.person.place_of_birth.svm';
var relExtractor = new mitie.RelationExtractor(nerFile, relFile);
 
//data = "Donald Trump says that Barack Obama was born in Kenya, not Hawaii.";
results = relExtractor.predict(data);

console.log(results);

/**
 * results => {
 *   tags:
 *    [ { tagNumber: 0, tagType: 'PERSON', tokens: 'Donald Trump' },
 *      { tagNumber: 0, tagType: 'PERSON', tokens: 'Barack Obama' },
 *      { tagNumber: 1, tagType: 'LOCATION', tokens: 'Kenya' },
 *      { tagNumber: 1, tagType: 'LOCATION', tokens: 'Hawaii' } ],
 *   relations:
 *    [ { score: 0.5990590608043147,
 *        first: 'Barack Obama',
 *        second: 'Kenya' } ] } 
 */