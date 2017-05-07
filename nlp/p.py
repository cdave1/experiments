import nltk
sent = "Albert Einstein spent many years at Princeton University in New Jersey"
sent1 = nltk.word_tokenize(sent)
sent2 = nltk.pos_tag(sent1)
sent3 = nltk.ne_chunk(sent2)
print sent3
