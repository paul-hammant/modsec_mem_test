#include <stdio.h>
#include <stdlib.h>
#include <modsecurity/modsecurity.h>
#include <sys/resource.h>
#include <modsecurity/rules_set.h>
#include <modsecurity/transaction.h>

#define ITERATIONS 1000000

int main() {
    ModSecurity *modsec;
    RulesSet *rules;  // Changed from RulesSet to Rules
    int i;

    modsec = msc_init();
    if (modsec == NULL) {
        fprintf(stderr, "Failed to initialize ModSecurity\n");
        return EXIT_FAILURE;
    }

    rules = msc_create_rules_set();  // Changed from msc_init_rules to msc_create_rules_set
    if (rules == NULL) {
        fprintf(stderr, "Failed to create rules set\n");
        msc_cleanup(modsec);
        return EXIT_FAILURE;
    }

    for (i = 0; i < ITERATIONS; i++) {
        Transaction *transaction = msc_new_transaction(modsec, rules, NULL);
        if (transaction == NULL) {
            fprintf(stderr, "Failed to create transaction\n");
            msc_rules_cleanup(rules);  // Changed from msc_cleanup_rules to msc_rules_cleanup
            msc_cleanup(modsec);
            return EXIT_FAILURE;
        }

        // Simulate processing a request
        msc_process_connection(transaction, "127.0.0.1", 12345, "127.0.0.1", 80);
        msc_process_uri(transaction, "http://example.com/large_payload", "POST", "1.1");
        // Simulate a larger request body
        char *large_body = malloc(1024 * 1024); // 1 MB
        for (int j = 0; j < 1024 * 1024; j++) {
            large_body[j] = 'A' + (rand() % 26); // Random character from A to Z
        }
        msc_append_request_body(transaction, large_body, 1024 * 1024);
        free(large_body);
        // Add random headers
        char header_name[20];
        for (int k = 0; k < 30; k++) { // Add 30 random headers
            int value_length = (k % 6 == 0) ? 8192 : 50; // Every 6th header has an 8KB value
            char *header_value = malloc(value_length + 1);
            snprintf(header_name, sizeof(header_name), "X-Random-Header-%d", k);
            for (int l = 0; l < value_length; l++) {
                header_value[l] = 'A' + (rand() % 26); // Random character from A to Z
            }
            header_value[value_length] = '\0';
            msc_add_request_header(transaction, header_name, header_value);
            free(header_value);
        }
        msc_process_request_headers(transaction);
        msc_process_request_body(transaction);
        msc_process_response_headers(transaction, 200, "HTTP 1.1");
        msc_process_response_body(transaction);
        msc_process_logging(transaction);

        msc_transaction_cleanup(transaction);

        if (i % 1000 == 0) {
            struct rusage usage;
            getrusage(RUSAGE_SELF, &usage);
            printf("Processed %d transactions, RAM usage: %ld KB\n", i, usage.ru_maxrss);
        }
    }

    msc_rules_cleanup(rules);  // Changed from msc_cleanup_rules to msc_rules_cleanup
    msc_cleanup(modsec);

    printf("Load test completed\n");
    return EXIT_SUCCESS;
}
