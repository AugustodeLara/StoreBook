import android.content.Context;
import android.content.Intent;

import androidx.test.core.app.ApplicationProvider;
import androidx.test.ext.junit.runners.AndroidJUnit4;

import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.util.ArrayList;
import java.util.List;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;

import android.view.LayoutInflater;
import android.view.View;


import com.example.bookStore.ui.main.adapter.BookAdapter;
import com.example.curlssl.R;

@RunWith(AndroidJUnit4.class)
public class BookAdapterTest {

    @Mock
    private Context mockContext;

    private BookAdapter bookAdapter;
    private List<String> books;

    @Before
    public void setUp() {
        MockitoAnnotations.initMocks(this);
        books = new ArrayList<>();
        bookAdapter = new BookAdapter(books, mockContext);
    }

    @Test
    public void addBook_ShouldAddBookToListAndNotifyItemInserted() {
        // Arrange
        String id = "123";
        String title = "Test Book";
        String description = "This is a test book";
        String thumbnailUrl = "http://books.google.com/books/content?id=cWK4CAAAQBAJ&printsec=frontcover&img=1&zoom=5&edge=curl&source=gbs_api";

        // Act
        bookAdapter.addBook(id, title, description, thumbnailUrl);

        // Assert
        assertEquals(4, books.size());  // Each book has 4 elements
        assertTrue(books.contains(id));
        assertTrue(books.contains(title));
        assertTrue(books.contains(description));
        assertTrue(books.contains(thumbnailUrl));
    }

    @Test
    public void removeDoubleAsterisks_ShouldRemoveAsterisks() {
        // Arrange
        String stringWithAsterisks = "HelloWorld**";

        // Act
        String result = bookAdapter.removeDoubleAsterisks(stringWithAsterisks);

        // Assert
        assertEquals("HelloWorld", result);
    }

    @Test
    public void testOnBindViewHolder_WithValidData_ShouldSetValuesAndOpenBookDetails() {
        // Arrange
        View itemView = LayoutInflater.from(ApplicationProvider.getApplicationContext())
                .inflate(R.layout.item_book, null);

        BookAdapter.ViewHolder viewHolder = new BookAdapter.ViewHolder(itemView);
        int position = 0;

        // Adicione alguns dados fictícios ao adaptador
        bookAdapter.addBook("1", "Title", "Description", "Thumbnail");

        // Capture a chamada ao método startActivity
        ArgumentCaptor<Intent> intentCaptor = ArgumentCaptor.forClass(Intent.class);

        // Act
        bookAdapter.onBindViewHolder(viewHolder, position);

        // Adicione um log para verificar se esse trecho de código é alcançado
        System.out.println("onBindViewHolder executed!");

        // Verifique se startActivity foi chamado apenas se a condição for atendida
        if (position * 6 + 5 < bookAdapter.getItemCount() * 6) {
            verify(mockContext).startActivity(intentCaptor.capture());

            // Aqui, você pode verificar detalhes específicos da Intent se necessário
            Intent capturedIntent = intentCaptor.getValue();
            assertNotNull(capturedIntent);

            // Não invoque a ação startActivity diretamente, apenas verifique se a chamada foi feita
        } else {
            // Se a condição não for atendida, não deveria chamar startActivity
            verify(mockContext, never()).startActivity(any(Intent.class));
        }
    }
}
