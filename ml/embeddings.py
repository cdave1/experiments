import random
import math

def text_to_embeddings_pipeline():
    """
    Shows the complete pipeline from raw text to word embeddings.
    This demonstrates tokenization, vocabulary building, and embedding lookup.
    """
    
    print("=" * 70)
    print("FROM RAW TEXT TO EMBEDDINGS - THE COMPLETE PIPELINE")
    print("=" * 70)
    
    # ============================================================
    # STEP 1: Start with raw text corpus
    # ============================================================
    
    raw_corpus = [
        "The cat sat on the mat.",
        "The dog sat on the log.",
        "A cat and a dog played.",
        "The mat was soft."
    ]
    
    print("\nSTEP 1: RAW TEXT CORPUS")
    print("-" * 40)
    for i, text in enumerate(raw_corpus):
        print(f"  Document {i+1}: {text}")
    
    # ============================================================
    # STEP 2: Tokenization - Split text into tokens
    # ============================================================
    
    def simple_tokenize(text):
        """
        Simple tokenizer that:
        1. Converts to lowercase
        2. Removes punctuation
        3. Splits on spaces
        
        Real tokenizers are much more sophisticated (BPE, WordPiece, etc.)
        """
        # Remove common punctuation and lowercase
        cleaned = text.lower().replace('.', '').replace(',', '').replace('!', '').replace('?', '')
        # Split into words
        return cleaned.split()
    
    print("\nSTEP 2: TOKENIZATION")
    print("-" * 40)
    
    tokenized_corpus = []
    for text in raw_corpus:
        tokens = simple_tokenize(text)
        tokenized_corpus.append(tokens)
        print(f"  '{text}' -> {tokens}")
    
    # ============================================================
    # STEP 3: Build Vocabulary
    # ============================================================
    
    print("\nSTEP 3: BUILD VOCABULARY")
    print("-" * 40)
    
    # Collect all unique tokens
    vocab_set = set()
    for tokens in tokenized_corpus:
        vocab_set.update(tokens)
    
    # Sort for consistency and add special tokens
    vocab_list = ['<PAD>', '<UNK>'] + sorted(list(vocab_set))
    
    # Create mappings
    token_to_id = {token: idx for idx, token in enumerate(vocab_list)}
    id_to_token = {idx: token for token, idx in token_to_id.items()}
    
    print(f"  Vocabulary size: {len(vocab_list)}")
    print(f"  Vocabulary: {vocab_list}")
    print(f"\n  Token to ID mapping:")
    for token, idx in token_to_id.items():
        print(f"    '{token}' -> {idx}")
    
    # ============================================================
    # STEP 4: Initialize Embedding Matrix
    # ============================================================
    
    print("\nSTEP 4: INITIALIZE EMBEDDING MATRIX")
    print("-" * 40)
    
    # Hyperparameters
    vocab_size = len(vocab_list)
    embedding_dim = 4  # Small for demonstration (typically 128-768 in real models)
    
    def initialize_embeddings(vocab_size, embedding_dim, method='random'):
        """
        Initialize the embedding matrix.
        In real training, these start random and are learned.
        """
        embeddings = []
        
        if method == 'random':
            # Random initialization (common approach)
            # Usually drawn from normal distribution with small variance
            for i in range(vocab_size):
                # Initialize with small random values
                embedding = [random.gauss(0, 0.5) for _ in range(embedding_dim)]
                embeddings.append(embedding)
        
        elif method == 'one_hot':
            # One-hot initialization (for demonstration)
            for i in range(vocab_size):
                embedding = [0] * embedding_dim
                if i < embedding_dim:
                    embedding[i] = 1
                embeddings.append(embedding)
        
        return embeddings
    
    # Create embedding matrix
    embedding_matrix = initialize_embeddings(vocab_size, embedding_dim, 'one_hot')
    
    print(f"  Embedding matrix shape: {vocab_size} tokens × {embedding_dim} dimensions")
    print(f"\n  Initial embeddings (randomly initialized):")
    for i in range(min(5, vocab_size)):  # Show first 5
        token = vocab_list[i]
        embedding = embedding_matrix[i]
        print(f"    {token:8} -> [{', '.join(f'{x:6.3f}' for x in embedding)}]")
    print("    ...")
    
    # ============================================================
    # STEP 5: Convert Text to Embeddings (Lookup)
    # ============================================================
    
    print("\nSTEP 5: TEXT TO EMBEDDINGS (THE LOOKUP PROCESS)")
    print("-" * 40)
    
    def text_to_embeddings(text, token_to_id, embedding_matrix):
        """
        Convert a text string to a sequence of embeddings.
        """
        # Tokenize
        tokens = simple_tokenize(text)
        
        # Convert tokens to IDs
        token_ids = []
        for token in tokens:
            if token in token_to_id:
                token_ids.append(token_to_id[token])
            else:
                # Use <UNK> token for out-of-vocabulary words
                token_ids.append(token_to_id['<UNK>'])
        
        # Look up embeddings
        embeddings = []
        for token_id in token_ids:
            embeddings.append(embedding_matrix[token_id])
        
        return tokens, token_ids, embeddings
    
    # Example: Convert a sentence to embeddings
    example_sentence = "The cat sat"
    tokens, token_ids, embeddings = text_to_embeddings(
        example_sentence, token_to_id, embedding_matrix
    )
    
    print(f"  Input text: '{example_sentence}'")
    print(f"  Tokens: {tokens}")
    print(f"  Token IDs: {token_ids}")
    print(f"\n  Embeddings:")
    for token, embedding in zip(tokens, embeddings):
        print(f"    {token:8} -> [{', '.join(f'{x:6.3f}' for x in embedding)}]")
    
    # ============================================================
    # STEP 6: How Embeddings Are Learned (Conceptual)
    # ============================================================
    
    print("\n" + "=" * 70)
    print("HOW EMBEDDINGS ARE LEARNED DURING TRAINING")
    print("=" * 70)
    
    print("""
The embedding matrix starts with random values, but during training:

1. FORWARD PASS:
   - Text -> Token IDs -> Embedding Lookup -> Transformer Layers -> Output
   
2. LOSS CALCULATION:
   - Compare model output to expected output (e.g., next word prediction)
   
3. BACKPROPAGATION:
   - The error signal flows backward through the network
   - Gradients tell us how to adjust each embedding to reduce error
   
4. UPDATE:
   - Embeddings are updated: embedding = embedding - learning_rate * gradient
   - Words that appear in similar contexts get similar gradients
   - Over time, similar words develop similar embeddings

EXAMPLE LEARNING SCENARIO:
- If the model sees "The cat sat" and "The dog sat" many times
- It learns that 'cat' and 'dog' appear in similar contexts
- Their embeddings gradually become more similar
- This captures semantic similarity!

KEY INSIGHTS:
- Each row in the embedding matrix is learned, not predetermined
- The lookup operation is differentiable (gradients can flow through it)
- Embeddings capture meaning through the contexts words appear in
- This is why "king - man + woman ≈ queen" works in trained embeddings!
""")
    
    # ============================================================
    # BONUS: Show position embeddings
    # ============================================================
    
    print("\nBONUS: POSITION EMBEDDINGS")
    print("-" * 40)
    
    def get_position_embedding(position, embedding_dim):
        """
        Simple sinusoidal position embedding (used in original Transformer).
        Real transformers might use learned position embeddings.
        """
        embedding = []
        for i in range(embedding_dim):
            if i % 2 == 0:
                # Even dimensions use sine
                embedding.append(math.sin(position / (10000 ** (i / embedding_dim))))
            else:
                # Odd dimensions use cosine
                embedding.append(math.cos(position / (10000 ** ((i-1) / embedding_dim))))
        return embedding
    
    print("  Position embeddings for our example sentence:")
    for i, token in enumerate(tokens):
        pos_embedding = get_position_embedding(i, embedding_dim)
        print(f"    Position {i} ({token}): [{', '.join(f'{x:6.3f}' for x in pos_embedding)}]")
    
    print("\n  Final input to transformer = token embedding + position embedding")
    print("  This gives the model both 'what' (token) and 'where' (position) information!")

# Run the demonstration
text_to_embeddings_pipeline()