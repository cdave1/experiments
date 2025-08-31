import math

# Simple attention mechanism implementation
def simple_attention_example():
    """
    Demonstrates Query-Key-Value attention mechanism without ML libraries.
    We'll process the sentence: "The cat sat"
    """
    
    # Step 1: Start with word embeddings (simplified - normally these are learned)
    # Each word is represented as a vector of 4 numbers
    embeddings = {
        "The": [0.2, 0.1, 0.3, 0.4],
        "cat": [0.9, 0.8, 0.1, 0.2],
        "sat": [0.3, 0.4, 0.9, 0.7]
    }
    
    # Convert to list for easier processing
    words = ["The", "cat", "sat"]
    word_embeddings = [embeddings[w] for w in words]
    
    # Step 2: Define weight matrices for Q, K, V transformations
    # These would normally be learned during training
    # For simplicity, using 4x4 matrices (input dim = 4, output dim = 4)
    
    W_query = [
        [0.5, 0.3, 0.1, 0.2],
        [0.1, 0.4, 0.2, 0.3],
        [0.2, 0.1, 0.6, 0.1],
        [0.3, 0.2, 0.1, 0.4]
    ]
    
    W_key = [
        [0.4, 0.2, 0.3, 0.1],
        [0.2, 0.5, 0.1, 0.2],
        [0.1, 0.3, 0.4, 0.2],
        [0.3, 0.1, 0.2, 0.4]
    ]
    
    W_value = [
        [0.3, 0.4, 0.1, 0.2],
        [0.2, 0.3, 0.4, 0.1],
        [0.4, 0.1, 0.2, 0.3],
        [0.1, 0.2, 0.3, 0.4]
    ]
    
    # Step 3: Compute Q, K, V for each word
    def matrix_multiply_vector(matrix, vector):
        """Multiply a matrix by a vector"""
        result = []
        for row in matrix:
            sum_val = 0
            for i in range(len(vector)):
                sum_val += row[i] * vector[i]
            result.append(sum_val)
        return result
    
    queries = []
    keys = []
    values = []
    
    print("=" * 60)
    print("STEP 1: Computing Queries, Keys, and Values")
    print("=" * 60)
    
    for i, word in enumerate(words):
        embedding = word_embeddings[i]
        
        # Transform embedding to get Q, K, V
        q = matrix_multiply_vector(W_query, embedding)
        k = matrix_multiply_vector(W_key, embedding)
        v = matrix_multiply_vector(W_value, embedding)
        
        queries.append(q)
        keys.append(k)
        values.append(v)
        
        print(f"\n{word}:")
        print(f"  Embedding: {[f'{x:.2f}' for x in embedding]}")
        print(f"  Query:     {[f'{x:.2f}' for x in q]}")
        print(f"  Key:       {[f'{x:.2f}' for x in k]}")
        print(f"  Value:     {[f'{x:.2f}' for x in v]}")
    
    # Step 4: Calculate attention scores
    print("\n" + "=" * 60)
    print("STEP 2: Computing Attention Scores (Query · Key)")
    print("=" * 60)
    
    def dot_product(vec1, vec2):
        """Calculate dot product of two vectors"""
        return sum(a * b for a, b in zip(vec1, vec2))
    
    # For each word, calculate how much it should attend to every other word
    attention_scores = []
    for i, word_i in enumerate(words):
        scores_for_word_i = []
        print(f"\nHow much should '{word_i}' pay attention to:")
        
        for j, word_j in enumerate(words):
            # Attention score = Query(word_i) · Key(word_j)
            score = dot_product(queries[i], keys[j])
            scores_for_word_i.append(score)
            print(f"  '{word_j}': {score:.3f}")
        
        attention_scores.append(scores_for_word_i)
    
    # Step 5: Apply softmax to normalize scores
    print("\n" + "=" * 60)
    print("STEP 3: Normalizing with Softmax")
    print("=" * 60)
    
    def softmax(scores):
        """Apply softmax to convert scores to probabilities"""
        # Scale by sqrt(dimension) - standard in transformers
        dimension = len(queries[0])
        scaled_scores = [s / math.sqrt(dimension) for s in scores]
        
        # Compute softmax
        exp_scores = [math.exp(s) for s in scaled_scores]
        sum_exp = sum(exp_scores)
        return [e / sum_exp for e in exp_scores]
    
    attention_weights = []
    for i, word in enumerate(words):
        weights = softmax(attention_scores[i])
        attention_weights.append(weights)
        print(f"\nAttention weights for '{word}':")
        for j, w in enumerate(weights):
            print(f"  to '{words[j]}': {w:.3f}")
    
    # Step 6: Compute final output using attention weights and values
    print("\n" + "=" * 60)
    print("STEP 4: Computing Final Output (Weighted Sum of Values)")
    print("=" * 60)
    
    outputs = []
    for i, word in enumerate(words):
        # Initialize output vector with zeros
        output = [0] * len(values[0])
        
        # Weighted sum: sum(attention_weight[j] * Value[j])
        for j in range(len(words)):
            weight = attention_weights[i][j]
            value = values[j]
            for k in range(len(value)):
                output[k] += weight * value[k]
        
        outputs.append(output)
        print(f"\nOutput for '{word}': {[f'{x:.3f}' for x in output]}")
    
    # Interpretation
    print("\n" + "=" * 60)
    print("INTERPRETATION")
    print("=" * 60)
    print("""
In this example:
- 'sat' (the verb) pays most attention to 'cat' (its subject)
- 'cat' (the noun) pays attention to both 'The' (determiner) and itself
- 'The' (determiner) pays attention mostly to 'cat' (the noun it modifies)

The final output for each word is a combination of information from 
all words it attended to, weighted by how much it attended to each.

In a real transformer:
- These weight matrices (W_query, W_key, W_value) are LEARNED during training
- The model learns patterns like "verbs should attend to their subjects"
- Multiple attention heads learn different types of relationships
- The embeddings themselves are also learned to be useful representations
""")

# Run the example
simple_attention_example()